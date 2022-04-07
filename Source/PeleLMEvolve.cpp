#include <PeleLM.H>

using namespace amrex;

void PeleLM::Evolve() {
   BL_PROFILE("PeleLM::Evolve()");

   bool do_not_evolve = ( (m_max_step == 0) ||
                          ((m_stop_time >= 0.) && (m_cur_time > m_stop_time)) );

   int plt_justDidIt = 0;
   int chk_justDidIt = 0;
   
   while(!do_not_evolve) {

      plt_justDidIt = 0;
      chk_justDidIt = 0;

      if (m_verbose > 0) {
         amrex::Print() << "\n ====================   NEW TIME STEP   ==================== \n";
      }

      if ( (m_regrid_int > 0) && (m_nstep > 0) && (m_nstep%m_regrid_int == 0) ) {
         if (m_verbose > 0) amrex::Print() << " Regridding...\n";
         regrid(0, m_cur_time);
         resetMacProjector();
         resetCoveredMask();
         updateDiagnostics();
      }

      int is_init = 0;
      Advance(is_init);
      m_nstep++;
      m_cur_time += m_dt;

      // Temporals
      if (doTemporalsNow()) {
         writeTemporals();
      }

      // Diagnostics
      doDiagnostics();

      // Check message
      bool dump_and_stop = checkMessage();

      // Check for plot file
      if (writePlotNow() || dump_and_stop) {
         WritePlotFile();
         plt_justDidIt = 1;
      }

      if (writeCheckNow() || dump_and_stop) {
         WriteCheckPointFile();
         chk_justDidIt = 1;
      }

      // Check for the end of the simulation
      do_not_evolve = ( (m_max_step >= 0 && m_nstep >= m_max_step) ||
                        (m_stop_time >= 0.0 && m_cur_time >= m_stop_time - 1.0e-12 * m_dt) ||
                        dump_and_stop );

   }

   if (m_verbose > 0) {
      amrex::Print() << "\n >> Final simulation time: " << m_cur_time << "\n";
   }
   if ( m_plot_int > 0 && !plt_justDidIt ) {
      WritePlotFile();
   }
   if ( m_check_int > 0 && !chk_justDidIt ) {
      WriteCheckPointFile();
   }
}

bool
PeleLM::writePlotNow()
{
   bool write_now = false;

   if ( m_plot_int > 0 && (m_nstep % m_plot_int == 0) ) {
      write_now = true;
   }
   // TODO : time controled

   return write_now;
}

bool
PeleLM::writeCheckNow()
{
   bool write_now = false;

   if ( m_check_int > 0 && (m_nstep % m_check_int == 0) ) {
      write_now = true;
   }
   // TODO : time controled ?

   return write_now;
}

bool
PeleLM::doTemporalsNow()
{
   bool write_now = false;

   if ( m_do_temporals &&
        (m_nstep % m_temp_int == 0) ) {
      write_now = true;
   }

   return write_now;
}

bool
PeleLM::checkMessage()
{
    bool dump_and_stop = false;
    if (m_nstep % m_message_int == 0) {
        int dumpclose = 0;
        if (ParallelDescriptor::IOProcessor()) {
            FILE *fp;
            if ( (fp=fopen("dump_and_stop","r")) != 0 ) {
                remove("dump_and_stop");
                dumpclose = 1;
                fclose(fp);
            }
        }
        int packed_data[1];
        packed_data[0] = dumpclose;
        ParallelDescriptor::Bcast(packed_data, 1, ParallelDescriptor::IOProcessorNumber());
        dump_and_stop = packed_data[0];
    }
    return dump_and_stop;
}
