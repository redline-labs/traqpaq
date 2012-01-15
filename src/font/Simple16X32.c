/******************************************************************************
 *
 * Simple 16x32 Font
 *
 * - Compiler:          GNU GCC for AVR32
 * - Supported devices: traq|paq hardware version 1.1
 * - AppNote:			N/A
 *
 * - Last Author:		Ryan David ( ryan.david@redline-electronics.com )
 *
 *
 * Copyright (c) 2012 Redline Electronics LLC.
 *
 * This file is part of traq|paq.
 *
 * traq|paq is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * traq|paq is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with traq|paq. If not, see http://www.gnu.org/licenses/.
 *
 ******************************************************************************/


const unsigned short FONT_SIMPLE16X32[] = {
	
/* character 0x20 (' '): (width=16, offset=0) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x21 ('!'): (width=16, offset=64) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0000, 0x0000, 0x0380, 0x0380, 
0x0380, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x22 ('"'): (width=16, offset=128) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x1CE0, 0x1CE0, 
0x1CE0, 0x1CE0, 0x1CE0, 0x1CE0, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x23 ('#'): (width=16, offset=192) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0638, 0x0638, 0x0638, 0x0638, 
0x3FFC, 0x3FFC, 0x0E70, 0x0E70, 
0x0C70, 0x0C70, 0x7FFC, 0x7FFC, 
0x0C70, 0x1CE0, 0x1CE0, 0x1CE0, 
0x1CE0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x24 ('$'): (width=16, offset=256) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0060, 0x00C0, 0x00C0, 
0x07F0, 0x0FF8, 0x1CC8, 0x38C0, 
0x38C0, 0x3880, 0x1F80, 0x0F80, 
0x07E0, 0x01F8, 0x01BC, 0x019C, 
0x031C, 0x031C, 0x2338, 0x3FF8, 
0x1FE0, 0x0300, 0x0300, 0x0600, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x25 ('%'): (width=16, offset=320) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x3C0C, 0x7E18, 
0xE738, 0xE730, 0xE760, 0xE760, 
0x7EC0, 0x3D80, 0x0180, 0x0300, 
0x0700, 0x0678, 0x0CFC, 0x1DCC, 
0x19CC, 0x31CC, 0x31CC, 0x60FC, 
0xC078, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x26 ('&'): (width=16, offset=384) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x07C0, 0x1FE0, 
0x3CF0, 0x3870, 0x3870, 0x3870, 
0x38E0, 0x1DC0, 0x1F80, 0x1E00, 
0x3F1C, 0x3B9C, 0x71DC, 0x71D8, 
0x70F8, 0x7070, 0x78F8, 0x3FFC, 
0x0F8C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x27 ('''): (width=16, offset=448) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x28 ('('): (width=16, offset=512) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0040, 0x00E0, 0x01C0, 
0x0380, 0x0300, 0x0700, 0x0E00, 
0x0E00, 0x0E00, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x0E00, 0x0E00, 0x0E00, 
0x0700, 0x0700, 0x0380, 0x01C0, 
0x00E0, 0x0040, 0x0000, 0x0000, 

/* character 0x29 (')'): (width=16, offset=576) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x2A ('*'): (width=16, offset=640) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x2B ('+'): (width=16, offset=704) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x2C (','): (width=16, offset=768) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x2D ('-'): (width=16, offset=832) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x2E ('.'): (width=16, offset=896) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x2F ('/'): (width=16, offset=960) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x30 ('0'): (width=16, offset=1024) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07C0, 0x1FF0, 0x1C78, 0x3838, 
0x3838, 0x703C, 0x707C, 0x71DC, 
0x739C, 0x771C, 0x7C1C, 0x781C, 
0x3838, 0x3838, 0x3C70, 0x1FF0, 
0x07C0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x31 ('1'): (width=16, offset=1088) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07C0, 0x0FC0, 0x1DC0, 0x19C0, 
0x01C0, 0x01C0, 0x01C0, 0x01C0, 
0x01C0, 0x01C0, 0x01C0, 0x01C0, 
0x01C0, 0x01C0, 0x01C0, 0x1FF8, 
0x1FF8, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x32 ('2'): (width=16, offset=1152) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0F80, 0x1FE0, 0x38E0, 0x3070, 
0x0070, 0x0070, 0x0070, 0x00E0, 
0x00E0, 0x01C0, 0x0380, 0x0700, 
0x0E00, 0x1C00, 0x3800, 0x3FF8, 
0x3FF8, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x33 ('3'): (width=16, offset=1216) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3F80, 0x3FE0, 0x00F0, 0x0070, 
0x0070, 0x0070, 0x00E0, 0x0FC0, 
0x0FE0, 0x00F0, 0x0038, 0x0038, 
0x0038, 0x0038, 0x00F0, 0x3FE0, 
0x3F80, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x34 ('4'): (width=16, offset=1280) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0038, 0x0078, 0x00F8, 0x01F8, 
0x03B8, 0x0738, 0x0E38, 0x1C38, 
0x3838, 0x7038, 0x7038, 0x7FFC, 
0x7FFC, 0x0038, 0x0038, 0x0038, 
0x0038, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x35 ('5'): (width=16, offset=1344) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3FF0, 0x3FF0, 0x3800, 0x3800, 
0x3800, 0x3800, 0x3800, 0x3FC0, 
0x3FF0, 0x00F8, 0x0038, 0x0038, 
0x0038, 0x0078, 0x00F0, 0x3FE0, 
0x3F80, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x36 ('6'): (width=16, offset=1408) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x01F8, 0x07F8, 0x0F00, 0x1E00, 
0x1C00, 0x1800, 0x3800, 0x3BF0, 
0x3FF8, 0x3C3C, 0x381C, 0x381C, 
0x381C, 0x1C1C, 0x1C38, 0x0FF0, 
0x07E0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x37 ('7'): (width=16, offset=1472) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3FFC, 0x3FFC, 0x001C, 0x0018, 
0x0038, 0x0030, 0x0070, 0x0060, 
0x00E0, 0x00C0, 0x01C0, 0x0180, 
0x0380, 0x0300, 0x0700, 0x0600, 
0x0E00, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x38 ('8'): (width=16, offset=1536) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07E0, 0x1FF8, 0x3C3C, 0x381C, 
0x381C, 0x3C1C, 0x1E78, 0x0FE0, 
0x07F0, 0x1E78, 0x1C3C, 0x381C, 
0x381C, 0x381C, 0x3C3C, 0x1FF8, 
0x07E0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x39 ('9'): (width=16, offset=1600) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07C0, 0x1FF0, 0x3C78, 0x7838, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x383C, 0x3FFC, 0x0FDC, 0x0018, 
0x0038, 0x0070, 0x00F0, 0x3FC0, 
0x3F00, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000,

/* character 0x3A (':'): (width=16, offset=1664) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x3B (';'): (width=16, offset=1728) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x3C ('<'): (width=16, offset=1792) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x3D ('='): (width=16, offset=1856) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x3E ('>'): (width=16, offset=1920) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x3F ('?'): (width=16, offset=1984) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x40 ('@'): (width=16, offset=2048) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x41 ('A'): (width=16, offset=2112) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0380, 0x03C0, 0x07C0, 0x07C0, 
0x06E0, 0x0EE0, 0x0EE0, 0x0C70, 
0x1C70, 0x1C70, 0x1830, 0x3FF8, 
0x3FF8, 0x3838, 0x701C, 0x701C, 
0x600C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x42 ('B'): (width=16, offset=2176) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3FE0, 0x3FF0, 0x3878, 0x3838, 
0x3838, 0x3838, 0x3870, 0x3FE0, 
0x3FF0, 0x3838, 0x381C, 0x381C, 
0x381C, 0x381C, 0x3838, 0x3FF0, 
0x3FE0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x43 ('C'): (width=16, offset=2240) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x03F8, 0x0FF8, 0x1E00, 0x3C00, 
0x3800, 0x7000, 0x7000, 0x7000, 
0x7000, 0x7000, 0x7000, 0x7000, 
0x3800, 0x3800, 0x1E00, 0x0FF8, 
0x07F8, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x44 ('D'): (width=16, offset=2304) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x7FC0, 0x7FE0, 0x7070, 0x7038, 
0x7038, 0x701C, 0x701C, 0x701C, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x7038, 0x7038, 0x70F0, 0x7FE0, 
0x7FC0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x45 ('E'): (width=16, offset=2368) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x1FF8, 0x1FF8, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1FF8, 
0x1FF8, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1FF8, 
0x1FF8, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x46 ('F'): (width=16, offset=2432) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x1FF8, 0x1FF8, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1FF8, 
0x1FF8, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x47 ('G'): (width=16, offset=2496) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x03FC, 0x0FFC, 0x1E00, 0x3C00, 
0x3800, 0x7800, 0x7000, 0x70FC, 
0x70FC, 0x701C, 0x701C, 0x701C, 
0x381C, 0x381C, 0x1E1C, 0x0FFC, 
0x03F0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x48 ('H'): (width=16, offset=2560) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x7038, 0x7038, 0x7038, 0x7038, 
0x7038, 0x7038, 0x7038, 0x7FF8, 
0x7FF8, 0x7038, 0x7038, 0x7038, 
0x7038, 0x7038, 0x7038, 0x7038, 
0x7038, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x49 ('I'): (width=16, offset=2624) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3FF8, 0x3FF8, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x3FF8, 
0x3FF8, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x4A ('J'): (width=16, offset=2688) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3FFE, 0x3FFE, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x30E0, 0x39C0, 0x1F80, 
0x0F00, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x4B ('K'): (width=16, offset=2752) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x381C, 0x383C, 0x3878, 0x38F0, 
0x39E0, 0x3BC0, 0x3F80, 0x3F00, 
0x3E00, 0x3F00, 0x3F80, 0x3BC0, 
0x39E0, 0x38F0, 0x3878, 0x383C, 
0x381C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x4C ('L'): (width=16, offset=2816) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x1C00, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1FF8, 
0x1FF8, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x4D ('M'): (width=16, offset=2880) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x783C, 0x7C7C, 0x7EFC, 0x7FFC, 
0x7FFC, 0x77DC, 0x739C, 0x711C, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x701C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x4E ('N'): (width=16, offset=2944) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x701C, 0x781C, 0x7C1C, 0x7C1C, 
0x7E1C, 0x7E1C, 0x7F1C, 0x771C, 
0x779C, 0x739C, 0x73DC, 0x71DC, 
0x71FC, 0x70FC, 0x707C, 0x707C, 
0x703C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x4F ('O'): (width=16, offset=3008) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07C0, 0x0FF0, 0x1C78, 0x3838, 
0x383C, 0x701C, 0x701C, 0x701C, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x7838, 0x3838, 0x3C70, 0x1FE0, 
0x07C0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x50 ('P'): (width=16, offset=0) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3FE0, 0x3FF0, 0x3878, 0x381C, 
0x381C, 0x381C, 0x381C, 0x383C, 
0x3878, 0x3FF0, 0x3FC0, 0x3800, 
0x3800, 0x3800, 0x3800, 0x3800, 
0x3800, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x51 ('Q'): (width=16, offset=64) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07E0, 0x0FF0, 0x1C38, 0x381C, 
0x381E, 0x700E, 0x700E, 0x700E, 
0x700E, 0x700E, 0x700E, 0x700E, 
0x781E, 0x381C, 0x3C38, 0x1FF0, 
0x07E0, 0x01C0, 0x01C0, 0x00E0, 
0x00FC, 0x003C, 0x0000, 0x0000, 

/* character 0x52 ('R'): (width=16, offset=128) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3FC0, 0x3FF0, 0x3878, 0x3838, 
0x3838, 0x3838, 0x3838, 0x3870, 
0x3FE0, 0x3FC0, 0x38E0, 0x3870, 
0x3870, 0x3838, 0x3838, 0x381C, 
0x381C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x53 ('S'): (width=16, offset=192) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07F0, 0x0FF8, 0x1C18, 0x3800, 
0x3800, 0x3800, 0x1E00, 0x1F80, 
0x07E0, 0x00F8, 0x003C, 0x001C, 
0x001C, 0x001C, 0x3038, 0x3FF0, 
0x1FE0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x54 ('T'): (width=16, offset=256) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x7FFC, 0x7FFC, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x55 ('U'): (width=16, offset=320) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x701C, 0x701C, 0x3838, 0x1FF0, 
0x0FC0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x56 ('V'): (width=16, offset=384) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x701C, 0x701C, 0x701C, 0x3018, 
0x3018, 0x3838, 0x1830, 0x1830, 
0x1C70, 0x1C60, 0x0C60, 0x0EE0, 
0x0EC0, 0x07C0, 0x07C0, 0x0380, 
0x0380, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x57 ('W'): (width=16, offset=448) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x701C, 0x701C, 0x711C, 0x739C, 
0x739C, 0x77DC, 0x7FFC, 0x7FFC, 
0x7EFC, 0x7C7C, 0x7C7C, 0x783C, 
0x3838, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x58 ('X'): (width=16, offset=512) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x701C, 0x3838, 0x1830, 0x1C70, 
0x0EE0, 0x07C0, 0x07C0, 0x0380, 
0x0380, 0x07C0, 0x0FE0, 0x0EE0, 
0x1C70, 0x3838, 0x3838, 0x701C, 
0xE00E, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x59 ('Y'): (width=16, offset=0) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x600C, 0x701C, 0x701C, 0x3838, 
0x3838, 0x1C70, 0x0C60, 0x0EE0, 
0x06C0, 0x07C0, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x5A ('Z'): (width=16, offset=64) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x7FF8, 0x7FF8, 0x0038, 0x0070, 
0x00E0, 0x00C0, 0x01C0, 0x0180, 
0x0300, 0x0700, 0x0600, 0x0C00, 
0x1C00, 0x1800, 0x3800, 0x7FFC, 
0x7FFC, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x5B ('['): (width=16, offset=128) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0FE0, 0x0FE0, 
0x0E00, 0x0E00, 0x0E00, 0x0E00, 
0x0E00, 0x0E00, 0x0E00, 0x0E00, 
0x0E00, 0x0E00, 0x0E00, 0x0E00, 
0x0E00, 0x0E00, 0x0E00, 0x0E00, 
0x0E00, 0x0E00, 0x0E00, 0x0E00, 
0x0FE0, 0x0FE0, 0x0000, 0x0000, 

/* character 0x5C ('\'): (width=16, offset=192) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x3000, 0x1800, 
0x1800, 0x1C00, 0x0C00, 0x0C00, 
0x0600, 0x0600, 0x0700, 0x0300, 
0x0300, 0x0180, 0x0180, 0x00C0, 
0x00C0, 0x00E0, 0x0060, 0x0060, 
0x0030, 0x0030, 0x0038, 0x0018, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x5D (']'): (width=16, offset=256) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0FE0, 0x0FE0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x0FE0, 0x0FE0, 0x0000, 0x0000, 

/* character 0x5E ('^'): (width=16, offset=320) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0380, 0x0380, 0x06C0, 0x0EE0, 
0x0C60, 0x1C70, 0x3838, 0x3838, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x5F ('_'): (width=16, offset=384) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0xFFFC, 0xFFFC, 0x0000, 0x0000, 

/* character 0x60 ('`'): (width=16, offset=448) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x1C00, 
0x0E00, 0x0700, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x61 ('a'): (width=16, offset=512) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07E0, 0x1FF8, 0x183C, 0x001C, 
0x001C, 0x07FC, 0x1FFC, 0x3C1C, 
0x381C, 0x381C, 0x3C7C, 0x1FFC, 
0x0F9C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x62 ('b'): (width=16, offset=576) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x3800, 0x3800, 
0x3800, 0x3800, 0x3800, 0x3800, 
0x39E0, 0x3BF8, 0x3E38, 0x3C1C, 
0x381C, 0x381C, 0x381C, 0x381C, 
0x381C, 0x3838, 0x3878, 0x3FF0, 
0x1FC0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x63 ('c'): (width=16, offset=640) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x03E0, 0x0FF0, 0x1E00, 0x1C00, 
0x3800, 0x3800, 0x3800, 0x3800, 
0x3800, 0x3C00, 0x1E00, 0x0FF0, 
0x07E0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x64 ('d'): (width=16, offset=704) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0038, 0x0038, 
0x0038, 0x0038, 0x0038, 0x0038, 
0x07F8, 0x1FF8, 0x3C38, 0x3838, 
0x7038, 0x7038, 0x7038, 0x7038, 
0x7038, 0x7078, 0x38F8, 0x3FB8, 
0x0F38, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x65 ('e'): (width=16, offset=768) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x03E0, 0x0FF0, 0x1C38, 0x181C, 
0x381C, 0x3FFC, 0x3FFC, 0x3800, 
0x3800, 0x3C00, 0x1E18, 0x0FF8, 
0x07F0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x66 ('f'): (width=16, offset=832) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x00FC, 0x03FC, 
0x0380, 0x0700, 0x0700, 0x0700, 
0x0700, 0x7FF8, 0x7FF8, 0x0700, 
0x0700, 0x0700, 0x0700, 0x0700, 
0x0700, 0x0700, 0x0700, 0x0700, 
0x0700, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x67 ('g'): (width=16, offset=896) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07FC, 0x1FFC, 0x3C78, 0x3838, 
0x3838, 0x3838, 0x3C78, 0x1FF0, 
0x3FC0, 0x3800, 0x3800, 0x3FF0, 
0x1FFC, 0x701C, 0x701C, 0x783C, 
0x3FF8, 0x1FE0, 0x0000, 0x0000,

/* character 0x68 ('h'): (width=16, offset=64) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x3800, 0x3800, 
0x3800, 0x3800, 0x3800, 0x3800, 
0x39E0, 0x3BF0, 0x3E78, 0x3C38, 
0x3838, 0x3838, 0x3838, 0x3838, 
0x3838, 0x3838, 0x3838, 0x3838, 
0x3838, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x69 ('i'): (width=16, offset=128) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x01C0, 
0x03E0, 0x01C0, 0x0000, 0x0000, 
0x01C0, 0x01C0, 0x01C0, 0x01C0, 
0x01C0, 0x01C0, 0x01C0, 0x01C0, 
0x01C0, 0x01C0, 0x01C0, 0x01C0, 
0x01C0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x6A ('j'): (width=16, offset=192) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x00C0, 
0x01E0, 0x00C0, 0x0000, 0x0000, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x00E0, 
0x00E0, 0x00E0, 0x00E0, 0x61C0, 
0x7F80, 0x3F00, 0x0000, 0x0000, 

/* character 0x6B ('k'): (width=16, offset=256) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x3800, 0x3800, 
0x3800, 0x3800, 0x3800, 0x3800, 
0x3838, 0x3870, 0x38E0, 0x39C0, 
0x3B80, 0x3F00, 0x3F80, 0x3B80, 
0x39C0, 0x38E0, 0x38E0, 0x3870, 
0x3838, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x6C ('l'): (width=16, offset=320) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x3FC0, 0x3FC0, 
0x01C0, 0x01C0, 0x01C0, 0x01C0, 
0x01C0, 0x01C0, 0x01C0, 0x01C0, 
0x01C0, 0x01C0, 0x01C0, 0x01C0, 
0x01C0, 0x01C0, 0x01C0, 0x3FF8, 
0x3FF8, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x6D ('m'): (width=16, offset=384) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x7738, 0x7FFC, 0x7BDC, 0x739C, 
0x739C, 0x739C, 0x739C, 0x739C, 
0x739C, 0x739C, 0x739C, 0x739C, 
0x739C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x6E ('n'): (width=16, offset=448) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x39E0, 0x3FF0, 0x3E78, 0x3C38, 
0x3838, 0x3838, 0x3838, 0x3838, 
0x3838, 0x3838, 0x3838, 0x3838, 
0x3838, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x6F ('o'): (width=16, offset=512) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07E0, 0x1FF0, 0x3C78, 0x383C, 
0x701C, 0x701C, 0x701C, 0x701C, 
0x701C, 0x3838, 0x3C78, 0x1FF0, 
0x0FC0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000,

/* character 0x70 ('p'): (width=16, offset=0) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x39E0, 0x3FF8, 0x3E38, 0x3C1C, 
0x381C, 0x381C, 0x381C, 0x381C, 
0x381C, 0x3838, 0x3878, 0x3FF0, 
0x3FC0, 0x3800, 0x3800, 0x3800, 
0x3800, 0x3800, 0x0000, 0x0000, 

/* character 0x71 ('q'): (width=16, offset=64) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07F8, 0x1FF8, 0x3C38, 0x3838, 
0x7038, 0x7038, 0x7038, 0x7038, 
0x7038, 0x7078, 0x38F8, 0x3FB8, 
0x0F38, 0x0038, 0x0038, 0x0038, 
0x0038, 0x0038, 0x0000, 0x0000, 

/* character 0x72 ('r'): (width=16, offset=128) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x1DF8, 0x1FFC, 0x1F0C, 0x1E00, 
0x1C00, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x1C00, 0x1C00, 0x1C00, 
0x1C00, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x73 ('s'): (width=16, offset=192) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x07F0, 0x1FF0, 0x3C00, 0x3800, 
0x3C00, 0x1F00, 0x0FE0, 0x01F0, 
0x0038, 0x0038, 0x3078, 0x3FF0, 
0x1FC0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x74 ('t'): (width=16, offset=256) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0700, 0x0700, 0x0700, 0x0700, 
0x7FF8, 0x7FF8, 0x0700, 0x0700, 
0x0700, 0x0700, 0x0700, 0x0700, 
0x0700, 0x0700, 0x0780, 0x03F8, 
0x01F8, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x75 ('u'): (width=16, offset=320) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3838, 0x3838, 0x3838, 0x3838, 
0x3838, 0x3838, 0x3838, 0x3838, 
0x3838, 0x3878, 0x3CF8, 0x1FF8, 
0x0F38, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x76 ('v'): (width=16, offset=384) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x6018, 0x7018, 0x3038, 0x3030, 
0x3830, 0x1860, 0x1C60, 0x0C60, 
0x0EC0, 0x06C0, 0x07C0, 0x07C0, 
0x0380, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x77 ('w'): (width=16, offset=448) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x300C, 0x300C, 0x300C, 0x300C, 
0x318C, 0x318C, 0x33CC, 0x33CC, 
0x366C, 0x366C, 0x3E7C, 0x1C38, 
0x1C38, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x78 ('x'): (width=16, offset=512) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x701C, 0x3838, 0x1C70, 0x0C60, 
0x0EE0, 0x07C0, 0x0380, 0x07C0, 
0x06E0, 0x0C60, 0x1C70, 0x3838, 
0x701C, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x79 ('y'): (width=16, offset=0) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x6018, 0x7018, 0x3038, 0x3830, 
0x1830, 0x1860, 0x1C60, 0x0C60, 
0x0EC0, 0x07C0, 0x0780, 0x0380, 
0x0380, 0x0300, 0x0700, 0x0E00, 
0xFC00, 0xF000, 0x0000, 0x0000, 

/* character 0x7A ('z'): (width=16, offset=64) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x3FF0, 0x3FF0, 0x0060, 0x00C0, 
0x01C0, 0x0180, 0x0300, 0x0600, 
0x0E00, 0x0C00, 0x1800, 0x3FF0, 
0x3FF0, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x7B ('{'): (width=16, offset=128) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x00F0, 0x01F0, 
0x03C0, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0780, 
0x3F00, 0x3F00, 0x0780, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x03C0, 
0x01F0, 0x00F0, 0x0000, 0x0000, 

/* character 0x7C ('|'): (width=16, offset=192) */
0x0000, 0x0000, 0x0000, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0000, 0x0000, 

/* character 0x7D ('}'): (width=16, offset=256) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x1E00, 0x1F00, 
0x0780, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x03C0, 
0x01F8, 0x01F8, 0x03C0, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0380, 
0x0380, 0x0380, 0x0380, 0x0780, 
0x1F00, 0x1E00, 0x0000, 0x0000, 

/* character 0x7E ('~'): (width=16, offset=320) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x1E00, 
0x3F0C, 0x718C, 0x70FC, 0x0078, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

/* character 0x7F (''): (width=16, offset=384) */
0x0000, 0x0000, 0x0000, 0x0000, 
0x0000, 0x0000, 0x7E00, 0x4200, 
0x4200, 0x4200, 0x4200, 0x4200, 
0x4200, 0x4200, 0x4200, 0x4200, 
0x4200, 0x4200, 0x4200, 0x4200, 
0x4200, 0x4200, 0x4200, 0x4200, 
0x4200, 0x7E00, 0x0000, 0x0000, 
0x0000, 0x0000, 0x0000, 0x0000, 

};