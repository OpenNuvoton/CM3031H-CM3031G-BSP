/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2026  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.56 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.  Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with the following terms:

The  software has  been  licensed by  SEGGER Software GmbH to Nuvoton
Technology  Corporation   at  the  address:   No. 4, Creation Rd. III,
Hsinchu Science Park,  Taiwan for  the purposes of  creating libraries
for its Arm Cortex-A,  Arm Cortex-M and  Arm9 32-bit microcontrollers,
commercialized and distributed by Nuvoton Technology Corporation under
the  terms and  conditions of an  End User License Agreement  supplied
with the libraries.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              Nuvoton Technology Corporation, No. 4, Creation Rd. III, Hsinchu Science Park, 30077 Hsinchu City, Taiwan
Licensed SEGGER software: emWin
License number:           GUI-00735
License model:            emWin License Agreement, signed February 27, 2018
Licensed platform:        Cortex-M, Cortex-A, and ARM9 32-bit series microcontroller designed and manufactured by Nuvoton Technology Corporation.
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2018-03-26 - 2026-09-04
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : GUI_DialogID.h
Purpose     : Widget IDs
---------------------------END-OF-HEADER------------------------------
*/

#ifndef  GUI_DIALOGID_H
#define  GUI_DIALOGID_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Dialog item IDs
*
*  The IDs below are arbitrary values. They just have to be unique
*  within the dialog.
*
*  If you use your own Ids, we recommend to use values above GUI_ID_USER.
*/
#define GUI_ID_OK                1
#define GUI_ID_CANCEL            2
#define GUI_ID_YES               3
#define GUI_ID_NO                4
#define GUI_ID_CLOSE             5
#define GUI_ID_HELP              6
#define GUI_ID_MAXIMIZE          7
#define GUI_ID_MINIMIZE          8

#define GUI_ID_VSCROLL    0xFE
#define GUI_ID_HSCROLL    0xFF

#define GUI_ID_VSCROLLER  0xFFE
#define GUI_ID_HSCROLLER  0xFFF

#define GUI_ID_EDIT0      0x100
#define GUI_ID_EDIT1      0x101
#define GUI_ID_EDIT2      0x102
#define GUI_ID_EDIT3      0x103
#define GUI_ID_EDIT4      0x104
#define GUI_ID_EDIT5      0x105
#define GUI_ID_EDIT6      0x106
#define GUI_ID_EDIT7      0x107
#define GUI_ID_EDIT8      0x108
#define GUI_ID_EDIT9      0x109

#define GUI_ID_LISTBOX0   0x110
#define GUI_ID_LISTBOX1   0x111
#define GUI_ID_LISTBOX2   0x112
#define GUI_ID_LISTBOX3   0x113
#define GUI_ID_LISTBOX4   0x114
#define GUI_ID_LISTBOX5   0x115
#define GUI_ID_LISTBOX6   0x116
#define GUI_ID_LISTBOX7   0x117
#define GUI_ID_LISTBOX8   0x118
#define GUI_ID_LISTBOX9   0x119

#define GUI_ID_CHECK0     0x120
#define GUI_ID_CHECK1     0x121
#define GUI_ID_CHECK2     0x122
#define GUI_ID_CHECK3     0x123
#define GUI_ID_CHECK4     0x124
#define GUI_ID_CHECK5     0x125
#define GUI_ID_CHECK6     0x126
#define GUI_ID_CHECK7     0x127
#define GUI_ID_CHECK8     0x128
#define GUI_ID_CHECK9     0x129

#define GUI_ID_SLIDER0    0x130
#define GUI_ID_SLIDER1    0x131
#define GUI_ID_SLIDER2    0x132
#define GUI_ID_SLIDER3    0x133
#define GUI_ID_SLIDER4    0x134
#define GUI_ID_SLIDER5    0x135
#define GUI_ID_SLIDER6    0x136
#define GUI_ID_SLIDER7    0x137
#define GUI_ID_SLIDER8    0x138
#define GUI_ID_SLIDER9    0x139

#define GUI_ID_SCROLLBAR0 0x140
#define GUI_ID_SCROLLBAR1 0x141
#define GUI_ID_SCROLLBAR2 0x142
#define GUI_ID_SCROLLBAR3 0x143
#define GUI_ID_SCROLLBAR4 0x144
#define GUI_ID_SCROLLBAR5 0x145
#define GUI_ID_SCROLLBAR6 0x146
#define GUI_ID_SCROLLBAR7 0x147
#define GUI_ID_SCROLLBAR8 0x148
#define GUI_ID_SCROLLBAR9 0x149

#define GUI_ID_RADIO0     0x150
#define GUI_ID_RADIO1     0x151
#define GUI_ID_RADIO2     0x152
#define GUI_ID_RADIO3     0x153
#define GUI_ID_RADIO4     0x154
#define GUI_ID_RADIO5     0x155
#define GUI_ID_RADIO6     0x156
#define GUI_ID_RADIO7     0x157
#define GUI_ID_RADIO8     0x158
#define GUI_ID_RADIO9     0x159

#define GUI_ID_TEXT0      0x160
#define GUI_ID_TEXT1      0x161
#define GUI_ID_TEXT2      0x162
#define GUI_ID_TEXT3      0x163
#define GUI_ID_TEXT4      0x164
#define GUI_ID_TEXT5      0x165
#define GUI_ID_TEXT6      0x166
#define GUI_ID_TEXT7      0x167
#define GUI_ID_TEXT8      0x168
#define GUI_ID_TEXT9      0x169

#define GUI_ID_BUTTON0    0x170
#define GUI_ID_BUTTON1    0x171
#define GUI_ID_BUTTON2    0x172
#define GUI_ID_BUTTON3    0x173
#define GUI_ID_BUTTON4    0x174
#define GUI_ID_BUTTON5    0x175
#define GUI_ID_BUTTON6    0x176
#define GUI_ID_BUTTON7    0x177
#define GUI_ID_BUTTON8    0x178
#define GUI_ID_BUTTON9    0x179

#define GUI_ID_DROPDOWN0  0x180
#define GUI_ID_DROPDOWN1  0x181
#define GUI_ID_DROPDOWN2  0x182
#define GUI_ID_DROPDOWN3  0x183
#define GUI_ID_DROPDOWN4  0x184
#define GUI_ID_DROPDOWN5  0x185
#define GUI_ID_DROPDOWN6  0x186
#define GUI_ID_DROPDOWN7  0x187
#define GUI_ID_DROPDOWN8  0x188
#define GUI_ID_DROPDOWN9  0x189

#define GUI_ID_MULTIEDIT0 0x190
#define GUI_ID_MULTIEDIT1 0x191
#define GUI_ID_MULTIEDIT2 0x192
#define GUI_ID_MULTIEDIT3 0x193
#define GUI_ID_MULTIEDIT4 0x194
#define GUI_ID_MULTIEDIT5 0x195
#define GUI_ID_MULTIEDIT6 0x196
#define GUI_ID_MULTIEDIT7 0x197
#define GUI_ID_MULTIEDIT8 0x198
#define GUI_ID_MULTIEDIT9 0x199

#define GUI_ID_LISTVIEW0  0x200
#define GUI_ID_LISTVIEW1  0x201
#define GUI_ID_LISTVIEW2  0x202
#define GUI_ID_LISTVIEW3  0x203
#define GUI_ID_LISTVIEW4  0x204
#define GUI_ID_LISTVIEW5  0x205
#define GUI_ID_LISTVIEW6  0x206
#define GUI_ID_LISTVIEW7  0x207
#define GUI_ID_LISTVIEW8  0x208
#define GUI_ID_LISTVIEW9  0x209

#define GUI_ID_PROGBAR0   0x210
#define GUI_ID_PROGBAR1   0x211
#define GUI_ID_PROGBAR2   0x212
#define GUI_ID_PROGBAR3   0x213
#define GUI_ID_PROGBAR4   0x214
#define GUI_ID_PROGBAR5   0x215
#define GUI_ID_PROGBAR6   0x216
#define GUI_ID_PROGBAR7   0x217
#define GUI_ID_PROGBAR8   0x218
#define GUI_ID_PROGBAR9   0x219

#define GUI_ID_GRAPH0     0x220
#define GUI_ID_GRAPH1     0x221
#define GUI_ID_GRAPH2     0x222
#define GUI_ID_GRAPH3     0x223
#define GUI_ID_GRAPH4     0x224
#define GUI_ID_GRAPH5     0x225
#define GUI_ID_GRAPH6     0x226
#define GUI_ID_GRAPH7     0x227
#define GUI_ID_GRAPH8     0x228
#define GUI_ID_GRAPH9     0x229

#define GUI_ID_MULTIPAGE0 0x230
#define GUI_ID_MULTIPAGE1 0x231
#define GUI_ID_MULTIPAGE2 0x232
#define GUI_ID_MULTIPAGE3 0x233
#define GUI_ID_MULTIPAGE4 0x234
#define GUI_ID_MULTIPAGE5 0x235
#define GUI_ID_MULTIPAGE6 0x236
#define GUI_ID_MULTIPAGE7 0x237
#define GUI_ID_MULTIPAGE8 0x238
#define GUI_ID_MULTIPAGE9 0x239

#define GUI_ID_TREEVIEW0  0x240
#define GUI_ID_TREEVIEW1  0x241
#define GUI_ID_TREEVIEW2  0x242
#define GUI_ID_TREEVIEW3  0x243
#define GUI_ID_TREEVIEW4  0x244
#define GUI_ID_TREEVIEW5  0x245
#define GUI_ID_TREEVIEW6  0x246
#define GUI_ID_TREEVIEW7  0x247
#define GUI_ID_TREEVIEW8  0x248
#define GUI_ID_TREEVIEW9  0x249

#define GUI_ID_ICONVIEW0  0x250
#define GUI_ID_ICONVIEW1  0x251
#define GUI_ID_ICONVIEW2  0x252
#define GUI_ID_ICONVIEW3  0x253
#define GUI_ID_ICONVIEW4  0x254
#define GUI_ID_ICONVIEW5  0x255
#define GUI_ID_ICONVIEW6  0x256
#define GUI_ID_ICONVIEW7  0x257
#define GUI_ID_ICONVIEW8  0x258
#define GUI_ID_ICONVIEW9  0x259

#define GUI_ID_LISTWHEEL0 0x260
#define GUI_ID_LISTWHEEL1 0x261
#define GUI_ID_LISTWHEEL2 0x262
#define GUI_ID_LISTWHEEL3 0x263
#define GUI_ID_LISTWHEEL4 0x264
#define GUI_ID_LISTWHEEL5 0x265
#define GUI_ID_LISTWHEEL6 0x266
#define GUI_ID_LISTWHEEL7 0x267
#define GUI_ID_LISTWHEEL8 0x268
#define GUI_ID_LISTWHEEL9 0x269

#define GUI_ID_IMAGE0     0x270
#define GUI_ID_IMAGE1     0x271
#define GUI_ID_IMAGE2     0x272
#define GUI_ID_IMAGE3     0x273
#define GUI_ID_IMAGE4     0x274
#define GUI_ID_IMAGE5     0x275
#define GUI_ID_IMAGE6     0x276
#define GUI_ID_IMAGE7     0x277
#define GUI_ID_IMAGE8     0x278
#define GUI_ID_IMAGE9     0x279

#define GUI_ID_SPINBOX0   0x280
#define GUI_ID_SPINBOX1   0x281
#define GUI_ID_SPINBOX2   0x282
#define GUI_ID_SPINBOX3   0x283
#define GUI_ID_SPINBOX4   0x284
#define GUI_ID_SPINBOX5   0x285
#define GUI_ID_SPINBOX6   0x286
#define GUI_ID_SPINBOX7   0x287
#define GUI_ID_SPINBOX8   0x288
#define GUI_ID_SPINBOX9   0x289

#define GUI_ID_CALENDAR0  0x290
#define GUI_ID_CALENDAR1  0x291
#define GUI_ID_CALENDAR2  0x292
#define GUI_ID_CALENDAR3  0x293
#define GUI_ID_CALENDAR4  0x294
#define GUI_ID_CALENDAR5  0x295
#define GUI_ID_CALENDAR6  0x296
#define GUI_ID_CALENDAR7  0x297
#define GUI_ID_CALENDAR8  0x298
#define GUI_ID_CALENDAR9  0x299

#define GUI_ID_KNOB0      0x300
#define GUI_ID_KNOB1      0x301
#define GUI_ID_KNOB2      0x302
#define GUI_ID_KNOB3      0x303
#define GUI_ID_KNOB4      0x304
#define GUI_ID_KNOB5      0x305
#define GUI_ID_KNOB6      0x306
#define GUI_ID_KNOB7      0x307
#define GUI_ID_KNOB8      0x308
#define GUI_ID_KNOB9      0x309

#define GUI_ID_ROTARY0    0x310
#define GUI_ID_ROTARY1    0x311
#define GUI_ID_ROTARY2    0x312
#define GUI_ID_ROTARY3    0x313
#define GUI_ID_ROTARY4    0x314
#define GUI_ID_ROTARY5    0x315
#define GUI_ID_ROTARY6    0x316
#define GUI_ID_ROTARY7    0x317
#define GUI_ID_ROTARY8    0x318
#define GUI_ID_ROTARY9    0x319

#define GUI_ID_SWIPELIST0 0x320
#define GUI_ID_SWIPELIST1 0x321
#define GUI_ID_SWIPELIST2 0x322
#define GUI_ID_SWIPELIST3 0x323
#define GUI_ID_SWIPELIST4 0x324
#define GUI_ID_SWIPELIST5 0x325
#define GUI_ID_SWIPELIST6 0x326
#define GUI_ID_SWIPELIST7 0x327
#define GUI_ID_SWIPELIST8 0x328
#define GUI_ID_SWIPELIST9 0x329

#define GUI_ID_SWITCH0    0x330
#define GUI_ID_SWITCH1    0x331
#define GUI_ID_SWITCH2    0x332
#define GUI_ID_SWITCH3    0x333
#define GUI_ID_SWITCH4    0x334
#define GUI_ID_SWITCH5    0x335
#define GUI_ID_SWITCH6    0x336
#define GUI_ID_SWITCH7    0x337
#define GUI_ID_SWITCH8    0x338
#define GUI_ID_SWITCH9    0x339

#define GUI_ID_GAUGE0     0x340
#define GUI_ID_GAUGE1     0x341
#define GUI_ID_GAUGE2     0x342
#define GUI_ID_GAUGE3     0x343
#define GUI_ID_GAUGE4     0x344
#define GUI_ID_GAUGE5     0x345
#define GUI_ID_GAUGE6     0x346
#define GUI_ID_GAUGE7     0x347
#define GUI_ID_GAUGE8     0x348
#define GUI_ID_GAUGE9     0x349

#define GUI_ID_QRCODE0    0x350
#define GUI_ID_QRCODE1    0x351
#define GUI_ID_QRCODE2    0x352
#define GUI_ID_QRCODE3    0x353
#define GUI_ID_QRCODE4    0x354
#define GUI_ID_QRCODE5    0x355
#define GUI_ID_QRCODE6    0x356
#define GUI_ID_QRCODE7    0x357
#define GUI_ID_QRCODE8    0x358
#define GUI_ID_QRCODE9    0x359

#define GUI_ID_KEYBOARD0  0x360
#define GUI_ID_KEYBOARD1  0x361
#define GUI_ID_KEYBOARD2  0x362
#define GUI_ID_KEYBOARD3  0x363
#define GUI_ID_KEYBOARD4  0x364
#define GUI_ID_KEYBOARD5  0x365
#define GUI_ID_KEYBOARD6  0x366
#define GUI_ID_KEYBOARD7  0x367
#define GUI_ID_KEYBOARD8  0x368
#define GUI_ID_KEYBOARD9  0x369

#define GUI_ID_ANIM0      0x370
#define GUI_ID_ANIM1      0x371
#define GUI_ID_ANIM2      0x372
#define GUI_ID_ANIM3      0x373
#define GUI_ID_ANIM4      0x374
#define GUI_ID_ANIM5      0x375
#define GUI_ID_ANIM6      0x376
#define GUI_ID_ANIM7      0x377
#define GUI_ID_ANIM8      0x378
#define GUI_ID_ANIM9      0x379

#define GUI_ID_SCROLLER0  0x380
#define GUI_ID_SCROLLER1  0x381
#define GUI_ID_SCROLLER2  0x382
#define GUI_ID_SCROLLER3  0x383
#define GUI_ID_SCROLLER4  0x384
#define GUI_ID_SCROLLER5  0x385
#define GUI_ID_SCROLLER6  0x386
#define GUI_ID_SCROLLER7  0x387
#define GUI_ID_SCROLLER8  0x388
#define GUI_ID_SCROLLER9  0x389

#define GUI_ID_WHEEL0     0x390
#define GUI_ID_WHEEL1     0x391
#define GUI_ID_WHEEL2     0x392
#define GUI_ID_WHEEL3     0x393
#define GUI_ID_WHEEL4     0x394
#define GUI_ID_WHEEL5     0x395
#define GUI_ID_WHEEL6     0x396
#define GUI_ID_WHEEL7     0x397
#define GUI_ID_WHEEL8     0x398
#define GUI_ID_WHEEL9     0x399

#define GUI_ID_MOVIE0     0x400
#define GUI_ID_MOVIE1     0x401
#define GUI_ID_MOVIE2     0x402
#define GUI_ID_MOVIE3     0x403
#define GUI_ID_MOVIE4     0x404
#define GUI_ID_MOVIE5     0x405
#define GUI_ID_MOVIE6     0x406
#define GUI_ID_MOVIE7     0x407
#define GUI_ID_MOVIE8     0x408
#define GUI_ID_MOVIE9     0x409

#define GUI_ID_TICKER0    0x410
#define GUI_ID_TICKER1    0x411
#define GUI_ID_TICKER2    0x412
#define GUI_ID_TICKER3    0x413
#define GUI_ID_TICKER4    0x414
#define GUI_ID_TICKER5    0x415
#define GUI_ID_TICKER6    0x416
#define GUI_ID_TICKER7    0x417
#define GUI_ID_TICKER8    0x418
#define GUI_ID_TICKER9    0x419

#define GUI_ID_USER       0x800

#if defined(__cplusplus)
}
#endif

#endif   /* ifdef GUI_DIALOGID_H */

/*************************** End of file ****************************/
