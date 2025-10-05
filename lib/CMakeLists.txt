cmake_minimum_required(VERSION 4.1)

set(CMAKE_C_FLAGS "-DPDC_WIDE -DPDC_FORCE_UTF8")

add_library(PDCurses STATIC
  PDCurses/curses.h
  PDCurses/curspriv.h
  PDCurses/panel.h

  PDCurses/common/acs437.h
  PDCurses/common/acsuni.h

  PDCurses/pdcurses/addch.c
  PDCurses/pdcurses/addchstr.c
  PDCurses/pdcurses/addstr.c
  PDCurses/pdcurses/attr.c
  PDCurses/pdcurses/beep.c
  PDCurses/pdcurses/bkgd.c
  PDCurses/pdcurses/border.c
  PDCurses/pdcurses/clear.c
  PDCurses/pdcurses/color.c
  PDCurses/pdcurses/debug.c
  PDCurses/pdcurses/delch.c
  PDCurses/pdcurses/deleteln.c
  PDCurses/pdcurses/getch.c
  PDCurses/pdcurses/getstr.c
  PDCurses/pdcurses/getyx.c
  PDCurses/pdcurses/inch.c
  PDCurses/pdcurses/inchstr.c
  PDCurses/pdcurses/initscr.c
  PDCurses/pdcurses/inopts.c
  PDCurses/pdcurses/insch.c
  PDCurses/pdcurses/insstr.c
  PDCurses/pdcurses/instr.c
  PDCurses/pdcurses/kernel.c
  PDCurses/pdcurses/keyname.c
  PDCurses/pdcurses/mouse.c
  PDCurses/pdcurses/move.c
  PDCurses/pdcurses/outopts.c
  PDCurses/pdcurses/overlay.c
  PDCurses/pdcurses/pad.c
  PDCurses/pdcurses/panel.c
  PDCurses/pdcurses/printw.c
  PDCurses/pdcurses/refresh.c
  PDCurses/pdcurses/scanw.c
  PDCurses/pdcurses/scr_dump.c
  PDCurses/pdcurses/scroll.c
  PDCurses/pdcurses/slk.c
  PDCurses/pdcurses/termattr.c
  PDCurses/pdcurses/touch.c
  PDCurses/pdcurses/util.c
  PDCurses/pdcurses/window.c

  PDCurses/wincon/pdcclip.c
  PDCurses/wincon/pdcdisp.c
  PDCurses/wincon/pdcgetsc.c
  PDCurses/wincon/pdckbd.c
  PDCurses/wincon/pdcscrn.c
  PDCurses/wincon/pdcsetsc.c
  PDCurses/wincon/pdcutil.c
  PDCurses/wincon/pdcwin.h
)

target_include_directories(PDCurses PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PDCurses)
