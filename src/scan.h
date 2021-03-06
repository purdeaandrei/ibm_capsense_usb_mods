/******************************************************************************
  Copyright 2014 Tom Wong-Cornall

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.  
 ******************************************************************************/
#ifndef SCAN_H
#define SCAN_H

#include <util/delay_basic.h>
#include "kbd.h"
#include "sr.h"

/* looks backwards, but only because LM339 outputs are open-collector */
#if defined(KBD_ACTIVE_LOW)
#	define KBD_ROW_TEST(PINx, Pxn) (PINx & (1 << Pxn))
#elif defined(KBD_ACTIVE_HIGH)
#	define KBD_ROW_TEST(PINx, Pxn) (!(PINx & (1 << Pxn)))
#endif

/* when changing these numbers, remember counting starts from (-)1 */
#define SCAN_DB_THRESH_TOP    11 // column-scan interrupt period * number of unskipped columns * (SCAN_DB_THRESH_TOP - 1) = steady state period
#define SCAN_DB_THRESH_BOTTOM -SCAN_DB_THRESH_TOP

extern volatile uint8_t scanTick;
extern volatile int8_t  scanStateAndAssociatedLayer[KBD_COLS][KBD_ROWS];
#define getScanState(col, row) (((int8_t)(scanStateAndAssociatedLayer[col][row] << 3)) >> 3)
#define setScanState(col, row, state) do { scanStateAndAssociatedLayer[col][row]=((scanStateAndAssociatedLayer[col][row] & 0xe0) | ((state) & 0x1f)); }  while (0)
#define isStickyLayer(col, row) (scanStateAndAssociatedLayer[col][row] & (1 << 7))
#define getStickyLayer(col, row) ((scanStateAndAssociatedLayer[col][row] >> 5) & 3)
#define setStickyLayer(col, row, layer) do { scanStateAndAssociatedLayer[col][row] = (scanStateAndAssociatedLayer[col][row] & 0x1f) | (1 << 7) | ((layer & 3) << 5); } while (0)
#define clearStickyLayer(col, row) do { scanStateAndAssociatedLayer[col][row] &= ~(1 << 7); } while (0)

void scanInit(void);
void scanEnable(void);
void scanDisable(void);
void scanPause(void);
void scanResume(void);

#endif
