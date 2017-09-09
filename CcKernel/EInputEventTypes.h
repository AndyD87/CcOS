/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcKernel
 * @subpage   EInputEventType
 *
 * @page      EInputEventType
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Enum EInputEventType
 */

/**
 * @defgroup EVT Input Event Type Definitions
 * @{
 */
enum class EInputEventType
{
  Undefined = 0,
  Keyboard = 1,
  Mouse = 2,
  Touch = 3,
  Joystick = 4,
};
/** @} */

enum class EMouseEventType
{
  Undefined = 0,
  LeftDown,
  LeftUp,
  MiddleDown,
  MiddleUp,
  RightDown,
  RightUp,
  ClickLeft,
  ClickMiddle,
  ClickRight,
  ClickDoubleLeft,
  ClickDoubleMiddle,
  ClickDoubleRight,
  Move,
  Hover,
  Leave,
};
/** @} */

/**
 * @defgroup EVV Input Event Values Definitions
 * @{
 */
#define EVC_UNDEFINED   0

#define EVC_KEY_0       1
#define EVC_KEY_1       2
#define EVC_KEY_2       3
#define EVC_KEY_3       4
#define EVC_KEY_4       5
#define EVC_KEY_5       6
#define EVC_KEY_6       7
#define EVC_KEY_7       8
#define EVC_KEY_8       9
#define EVC_KEY_9      10

#define EVC_KEY_A       0x01
#define EVC_KEY_B       0x01
#define EVC_KEY_C       0x01
#define EVC_KEY_D       0x01
#define EVC_KEY_E       0x01
#define EVC_KEY_F       0x01
#define EVC_KEY_G       0x01
#define EVC_KEY_H       0x01
#define EVC_KEY_I       0x01
#define EVC_KEY_J       0x01
#define EVC_KEY_K       0x01
#define EVC_KEY_L       0x01
#define EVC_KEY_M       0x01
#define EVC_KEY_N       0x01
#define EVC_KEY_O       0x01
#define EVC_KEY_P       0x01
#define EVC_KEY_Q       0x01
#define EVC_KEY_R       0x01
#define EVC_KEY_S       0x01
#define EVC_KEY_T       0x01
#define EVC_KEY_U       0x01
#define EVC_KEY_V       0x01
#define EVC_KEY_W       0x01
#define EVC_KEY_X       0x01
#define EVC_KEY_Y       0x01
#define EVC_KEY_Z       0x01

#define EVC_KEY_NUM_0      0x01
#define EVC_KEY_NUM_1      0x01
#define EVC_KEY_NUM_2      0x01
#define EVC_KEY_NUM_3      0x01
#define EVC_KEY_NUM_4      0x01
#define EVC_KEY_NUM_5      0x01
#define EVC_KEY_NUM_6      0x01
#define EVC_KEY_NUM_7      0x01
#define EVC_KEY_NUM_8      0x01
#define EVC_KEY_NUM_9      0x01
#define EVC_KEY_NUM_PLUS   0x01
#define EVC_KEY_NUM_MINUS  0x01
#define EVC_KEY_NUM_DIV    0x01
#define EVC_KEY_NUM_MUL    0x01
#define EVC_KEY_NUM_ENTER  0x01
#define EVC_KEY_NUM_DOT    0x01
/** @}*/
