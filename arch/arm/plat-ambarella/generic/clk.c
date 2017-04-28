/*
 * arch/arm/plat-ambarella/generic/clk.c
 *
 * Author: Anthony Ginger <hfjiang@ambarella.com>
 *
 * Copyright (C) 2004-2010, Ambarella, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/io.h>
#include <linux/export.h>
#include <linux/proc_fs.h>
#include <linux/clk.h>

#include <asm/uaccess.h>

#include <mach/hardware.h>
#include <plat/clk.h>

/* ==========================================================================*/
static LIST_HEAD(ambarella_all_clocks);
DEFINE_MUTEX(ambarella_clock_mutex);

/* ==========================================================================*/
const struct pll_table_s ambarella_rct_pll_table[AMBARELLA_RCT_PLL_TABLE_SIZE] =
{
	{    16601562500000,	16,	 1,	15,	128},
	{    16732282936573,	16,	 1,	15,	127},
	{    16865080222486,	16,	 1,	15,	126},
	{    17000000923872,	16,	 1,	15,	125},
	{    17137097194791,	16,	 1,	15,	124},
	{    17276423051953,	16,	 1,	15,	123},
	{    17418032512068,	16,	 1,	15,	122},
	{    17561983317137,	16,	 1,	15,	121},
	{    17708333209157,	16,	 1,	15,	120},
	{    17847768962383,	16,	 1,	15,	119},
	{    17989417538047,	16,	 1,	15,	118},
	{    18133332952857,	16,	 1,	15,	117},
	{    18279569223523,	16,	 1,	15,	116},
	{    18428184092045,	16,	 1,	15,	115},
	{    18579235300422,	16,	 1,	15,	114},
	{    18732782453299,	16,	 1,	15,	113},
	{    18973214551806,	16,	 1,	15,	112},
	{    19122609868646,	16,	 1,	15,	111},
	{    19274376332760,	16,	 1,	15,	110},
	{    19428571686149,	16,	 1,	15,	109},
	{    19585253670812,	16,	 1,	15,	108},
	{    19744483754039,	16,	 1,	15,	107},
	{    19906323403120,	16,	 1,	15,	106},
	{    20070837810636,	17,	 1,	15,	112},
	{    20238095894456,	16,	 1,	15,	105},
	{    20432692021132,	16,	 1,	15,	104},
	{    20593579858541,	16,	 1,	15,	103},
	{    20757021382451,	16,	 1,	15,	102},
	{    20923076197505,	16,	 1,	15,	101},
	{    21091811358929,	17,	 1,	15,	106},
	{    21263290196657,	16,	 1,	15,	100},
	{    21437577903271,	16,	 1,	15,	 99},
	{    21614748984575,	16,	 1,	15,	 98},
	{    21794872358441,	16,	 1,	15,	 97},
	{    22135416045785,	16,	 1,	15,	 96},
	{    22309711202979,	16,	 1,	15,	 95},
	{    22486772388220,	16,	 1,	15,	 94},
	{    22666666656733,	17,	 1,	15,	 99},
	{    22849462926388,	16,	 1,	15,	 93},
	{    23035230115056,	16,	 1,	15,	 92},
	{    23224044591188,	16,	 1,	15,	 91},
	{    23415977135301,	17,	 1,	15,	 96},
	{    23611111566424,	16,	 1,	15,	 90},
	{    23809524253011,	16,	 1,	15,	 89},
	{    24147726595402,	16,	 1,	15,	 88},
	{    24337867274880,	16,	 1,	15,	 87},
	{    24531025439501,	16,	 1,	15,	 86},
	{    24727271869779,	16,	 1,	14,	 91},
	{    24926686659455,	16,	 1,	15,	 85},
	{    25129342451692,	16,	 1,	15,	 84},
	{    25335321202874,	16,	 1,	14,	 89},
	{    25544703006744,	16,	 1,	15,	 83},
	{    25757575407624,	16,	 1,	15,	 82},
	{    25974025949836,	17,	 1,	15,	 86},
	{    26194144040346,	16,	 1,	15,	 81},
	{    26562500745058,	16,	 1,	15,	 80},
	{    26771653443575,	16,	 1,	15,	 79},
	{    26984127238393,	17,	 1,	15,	 83},
	{    27200000360608,	16,	 1,	15,	 78},
	{    27419354766607,	16,	 1,	15,	 77},
	{    27642276138067,	17,	 1,	15,	 81},
	{    27868852019310,	16,	 1,	15,	 76},
	{    28099173679948,	17,	 1,	15,	 80},
	{    28333334252238,	16,	 1,	15,	 75},
	{    28571428731084,	16,	 1,	15,	 74},
	{    28813559561968,	17,	 1,	15,	 78},
	{    29059829190373,	16,	 1,	15,	 73},
	{    29513888061047,	16,	 1,	15,	 72},
	{    29746280983090,	16,	 1,	15,	 71},
	{    29982363805175,	17,	 1,	15,	 75},
	{    30222222208977,	16,	 1,	15,	 70},
	{    30465949326754,	16,	 1,	14,	 74},
	{    30713640153408,	16,	 1,	15,	 69},
	{    30965391546488,	16,	 1,	14,	 73},
	{    31221304088831,	16,	 1,	15,	 68},
	{    31481482088566,	16,	 1,	15,	 67},
	{    31746033579111,	16,	 1,	14,	 71},
	{    32015066593885,	16,	 1,	15,	 66},
	{    32288700342177,	16,	 1,	14,	 70},
	{    32567050307989,	16,	 1,	15,	 65},
	{    32850243151188,	17,	 1,	15,	 68},
	{    33203125000000,	16,	 1,	15,	 64},
	{    33464565873146,	17,	 1,	15,	 67},
	{    33730160444975,	16,	 1,	15,	 63},
	{    34000001847744,	17,	 1,	15,	 66},
	{    34274194389582,	16,	 1,	15,	 62},
	{    34552846103907,	17,	 1,	15,	 65},
	{    34836065024136,	16,	 1,	15,	 61},
	{    35123966634274,	17,	 1,	15,	 64},
	{    35416666418314,	16,	 1,	15,	 60},
	{    35714287310839,	16,	 1,	14,	 63},
	{    36016948521137,	16,	 1,	15,	 59},
	{    36324787884951,	16,	 1,	14,	 62},
	{    36637932062149,	16,	 1,	15,	 58},
	{    36956522613764,	16,	 1,	14,	 61},
	{    37280701100826,	16,	 1,	15,	 57},
	{    37610620260239,	16,	 1,	14,	 60},
	{    37946429103613,	16,	 1,	15,	 56},
	{    38245219737291,	16,	 1,	14,	 59},
	{    38548752665520,	16,	 1,	15,	 55},
	{    38857143372297,	16,	 1,	14,	 58},
	{    39170507341623,	16,	 1,	15,	 54},
	{    39488967508078,	16,	 1,	14,	 57},
	{    39812646806240,	16,	 1,	15,	 53},
	{    40141675621271,	17,	 1,	15,	 56},
	{    40476191788912,	16,	 1,	14,	 56},
	{    40816325694323,	16,	 1,	15,	 52},
	{    41162226349115,	16,	 1,	14,	 55},
	{    41514042764902,	16,	 1,	15,	 51},
	{    41871920228004,	16,	 1,	14,	 54},
	{    42236026376486,	16,	 1,	15,	 50},
	{    42606517672539,	16,	 1,	14,	 53},
	{    42983565479517,	17,	 1,	15,	 52},
	{    43367348611355,	16,	 1,	15,	 49},
	{    43758042156696,	18,	 1,	15,	 54},
	{    44270832091570,	16,	 1,	15,	 48},
	{    44619422405958,	18,	 1,	15,	 53},
	{    44973544776440,	16,	 1,	15,	 47},
	{    45333333313465,	16,	 1,	14,	 50},
	{    45698925852776,	17,	 1,	15,	 49},
	{    46070460230112,	16,	 1,	15,	 46},
	{    46448089182377,	18,	 1,	15,	 51},
	{    46831954270601,	17,	 1,	15,	 48},
	{    47222223132849,	16,	 1,	15,	 45},
	{    47619048506021,	17,	 1,	15,	 47},
	{    48022598028183,	16,	 1,	15,	 44},
	{    48433046787977,	18,	 1,	15,	 49},
	{    48850573599339,	17,	 1,	15,	 46},
	{    49275361001492,	16,	 1,	15,	 43},
	{    49707602709532,	17,	 1,	15,	 45},
	{    50147492438555,	16,	 1,	14,	 45},
	{    50595238804817,	16,	 1,	15,	 42},
	{    51051050424576,	17,	 1,	15,	 44},
	{    51515150815248,	16,	 1,	15,	 41},
	{    51987767219543,	17,	 1,	15,	 43},
	{    52469134330750,	16,	 1,	14,	 43},
	{    53125001490116,	16,	 1,	15,	 40},
	{    53543306887150,	17,	 1,	15,	 42},
	{    53968254476786,	16,	 1,	14,	 42},
	{    54400000721216,	16,	 1,	15,	 39},
	{    54838709533215,	17,	 1,	15,	 41},
	{    55284552276134,	16,	 1,	14,	 41},
	{    55737704038620,	16,	 1,	15,	 38},
	{    56198347359896,	17,	 1,	15,	 40},
	{    56666668504477,	16,	 1,	14,	 40},
	{    57142857462168,	16,	 1,	15,	 37},
	{    57627119123936,	17,	 1,	15,	 39},
	{    58119658380747,	16,	 1,	14,	 39},
	{    58620691299438,	16,	 1,	15,	 36},
	{    59130433946848,	17,	 1,	15,	 38},
	{    59649121016264,	16,	 1,	14,	 38},
	{    60176990926266,	18,	 1,	14,	 42},
	{    60714285820723,	16,	 1,	15,	 35},
	{    61261262744665,	16,	 1,	14,	 37},
	{    61818182468414,	16,	 1,	13,	 39},
	{    62385320663452,	16,	 1,	15,	 34},
	{    62962964177132,	16,	 1,	14,	 36},
	{    63551403582096,	16,	 1,	13,	 38},
	{    64150944352150,	16,	 1,	15,	 33},
	{    64761906862258,	16,	 1,	14,	 35},
	{    65384618937969,	16,	 1,	13,	 37},
	{    65891474485397,	17,	 1,	15,	 34},
	{    66406250000000,	16,	 1,	15,	 32},
	{    66929131746292,	16,	 1,	12,	 39},
	{    67460320889950,	18,	 1,	15,	 35},
	{    68000003695488,	17,	 1,	15,	 33},
	{    68548388779163,	16,	 1,	15,	 31},
	{    69105692207813,	16,	 1,	13,	 35},
	{    69672130048274,	18,	 1,	15,	 34},
	{    70247933268547,	17,	 1,	15,	 32},
	{    70833332836628,	16,	 1,	15,	 30},
	{    71428574621677,	16,	 1,	13,	 34},
	{    72033897042274,	18,	 1,	15,	 33},
	{    72649575769901,	16,	 1,	14,	 31},
	{    73275864124298,	16,	 1,	15,	 29},
	{    73913045227528,	18,	 1,	15,	 32},
	{    74561402201653,	17,	 1,	15,	 30},
	{    75221240520477,	16,	 1,	14,	 30},
	{    75892858207226,	16,	 1,	15,	 28},
	{    76576575636864,	18,	 1,	15,	 31},
	{    77272728085518,	17,	 1,	15,	 29},
	{    77981650829315,	16,	 1,	14,	 29},
	{    78703701496124,	16,	 1,	15,	 27},
	{    79439252614975,	17,	 1,	14,	 30},
	{    80188676714897,	17,	 1,	15,	 28},
	{    80952383577824,	16,	 1,	14,	 28},
	{    81730768084526,	16,	 1,	15,	 26},
	{    82524269819260,	17,	 1,	14,	 29},
	{    83333335816860,	16,	 1,	14,	 27},
	{    84158413112164,	18,	 1,	14,	 30},
	{    85000000894070,	16,	 1,	15,	 25},
	{    85858583450317,	17,	 1,	14,	 28},
	{    86734697222710,	16,	 1,	14,	 26},
	{    87628863751888,	18,	 1,	15,	 27},
	{    88541664183140,	16,	 1,	15,	 24},
	{    89238844811916,	19,	 1,	15,	 28},
	{    89947089552879,	17,	 1,	15,	 25},
	{    90666666626930,	16,	 1,	14,	 25},
	{    91397851705551,	17,	 1,	13,	 28},
	{    92140920460224,	16,	 1,	15,	 23},
	{    92896178364754,	16,	 1,	13,	 26},
	{    93663908541203,	17,	 1,	15,	 24},
	{    94444446265697,	16,	 1,	14,	 24},
	{    95238097012043,	17,	 1,	13,	 27},
	{    96045196056366,	16,	 1,	15,	 22},
	{    96866093575954,	16,	 1,	13,	 25},
	{    97701147198677,	17,	 1,	15,	 23},
	{    98550722002983,	16,	 1,	14,	 23},
	{    99415205419064,	17,	 1,	14,	 24},
	{   100294984877110,	16,	 1,	12,	 26},
	{   101190477609634,	16,	 1,	15,	 21},
	{   102102100849152,	17,	 1,	15,	 22},
	{   103030301630497,	16,	 1,	14,	 22},
	{   103975534439087,	17,	 1,	14,	 23},
	{   104938268661499,	16,	 1,	13,	 23},
	{   105919003486633,	16,	 1,	15,	 20},
	{   106918238103390,	17,	 1,	15,	 21},
	{   107936508953571,	16,	 1,	14,	 21},
	{   108974359929562,	17,	 1,	14,	 22},
	{   110032364726067,	16,	 1,	13,	 22},
	{   111111111938953,	16,	 1,	15,	 19},
	{   112211219966412,	17,	 1,	15,	 20},
	{   113333337008953,	16,	 1,	14,	 20},
	{   114478111267090,	17,	 1,	14,	 21},
	{   115646257996559,	16,	 1,	13,	 21},
	{   116838485002518,	17,	 1,	13,	 22},
	{   118055552244186,	16,	 1,	15,	 18},
	{   119298242032528,	16,	 1,	14,	 19},
	{   120567373931408,	16,	 1,	13,	 20},
	{   121863797307014,	17,	 1,	13,	 21},
	{   123188406229019,	18,	 1,	13,	 22},
	{   124542124569416,	16,	 1,	15,	 17},
	{   125925928354263,	16,	 1,	14,	 18},
	{   127340823411942,	16,	 1,	13,	 19},
	{   128787875175475,	17,	 1,	13,	 20},
	{   130268201231956,	16,	 1,	12,	 20},
	{   131782948970795,	17,	 1,	15,	 17},
	{   132812500000000,	16,	 1,	15,	 16},
	{   133858263492584,	16,	 1,	10,	 23},
	{   134920641779900,	16,	 1,	13,	 18},
	{   136000007390976,	19,	 1,	13,	 21},
	{   137096777558327,	16,	 1,	12,	 19},
	{   138211384415627,	19,	 1,	15,	 18},
	{   139344260096549,	18,	 1,	15,	 17},
	{   140495866537094,	17,	 1,	15,	 16},
	{   141666665673256,	16,	 1,	15,	 15},
	{   142857149243355,	16,	 1,	13,	 17},
	{   144067794084549,	21,	 1,	15,	 19},
	{   145299151539803,	16,	 1,	12,	 18},
	{   146551728248596,	19,	 1,	15,	 17},
	{   147826090455055,	18,	 1,	15,	 16},
	{   149122804403305,	17,	 1,	15,	 15},
	{   150442481040955,	16,	 1,	14,	 15},
	{   151785716414452,	16,	 1,	15,	 14},
	{   153153151273727,	16,	 1,	12,	 17},
	{   154545456171036,	20,	 1,	15,	 17},
	{   155963301658630,	19,	 1,	15,	 16},
	{   157407402992249,	18,	 1,	15,	 15},
	{   158878505229950,	17,	 1,	14,	 15},
	{   160377353429794,	17,	 1,	15,	 14},
	{   161904767155647,	16,	 1,	14,	 14},
	{   163461536169052,	16,	 1,	15,	 13},
	{   165048539638519,	20,	 1,	14,	 17},
	{   166666671633720,	19,	 1,	15,	 15},
	{   168316826224327,	18,	 1,	14,	 15},
	{   170000001788139,	18,	 1,	15,	 14},
	{   171717166900635,	17,	 1,	14,	 14},
	{   173469394445419,	16,	 1,	14,	 13},
	{   175257727503777,	20,	 1,	15,	 15},
	{   177083328366280,	16,	 1,	15,	 12},
	{   178947374224663,	19,	 1,	15,	 14},
	{   180851057171822,	18,	 1,	14,	 14},
	{   182795703411102,	18,	 1,	15,	 13},
	{   184782609343529,	17,	 1,	14,	 13},
	{   186813190579414,	17,	 1,	15,	 12},
	{   188888892531395,	16,	 1,	14,	 12},
	{   191011235117912,	19,	 1,	15,	 13},
	{   193181812763214,	16,	 1,	15,	 11},
	{   195402294397354,	18,	 1,	14,	 13},
	{   197674423456192,	18,	 1,	15,	 12},
	{   200000002980232,	17,	 1,	14,	 12},
	{   202380955219269,	16,	 1,	13,	 12},
	{   204819276928902,	16,	 1,	14,	 11},
	{   207317069172859,	19,	 1,	15,	 12},
	{   209876537322998,	18,	 1,	14,	 12},
	{   212500005960464,	16,	 1,	15,	 10},
	{   215189874172211,	18,	 1,	15,	 11},
	{   217948719859123,	17,	 1,	14,	 11},
	{   220779225230217,	16,	 1,	13,	 11},
	{   223684206604958,	17,	 1,	15,	 10},
	{   226666674017906,	16,	 1,	14,	 10},
	{   229729726910591,	18,	 1,	14,	 11},
	{   232876718044281,	17,	 1,	13,	 11},
	{   236111104488373,	16,	 1,	15,	  9},
	{   239436626434326,	17,	 1,	14,	 10},
	{   242857143282890,	16,	 1,	13,	 10},
	{   246376812458038,	18,	 1,	13,	 11},
	{   250000000000000,	16,	 1,	14,	  9},
	{   253731340169907,	18,	 1,	14,	 10},
	{   255639106035233,	17,	 1,	13,	 10},
	{   257575750350951,	16,	 1,	11,	 11},
	{   261538475751877,	16,	 1,	12,	 10},
	{   263565897941589,	18,	 1,	15,	  9},
	{   265625000000000,	16,	 1,	15,	  8},
	{   267716526985168,	20,	 1,	12,	 12},
	{   269841283559798,	16,	 1,	13,	  9},
	{   272000014781952,	17,	 1,	11,	 11},
	{   274193555116653,	21,	 1,	15,	 10},
	{   276422768831253,	19,	 1,	15,	  9},
	{   278688520193099,	20,	 1,	14,	 10},
	{   280991733074188,	17,	 1,	15,	  8},
	{   283333331346512,	16,	 1,	14,	  8},
	{   285714298486710,	17,	 1,	13,	  9},
	{   288135588169098,	18,	 1,	11,	 11},
	{   290598303079605,	16,	 1,	12,	  9},
	{   293103456497192,	21,	 1,	14,	 10},
	{   295652180910110,	18,	 1,	15,	  8},
	{   298245608806610,	17,	 1,	14,	  8},
	{   300884962081909,	18,	 1,	13,	  9},
	{   303571432828903,	16,	 1,	15,	  7},
	{   306306302547455,	17,	 1,	12,	  9},
	{   309090912342072,	20,	 1,	14,	  9},
	{   311926603317261,	19,	 1,	15,	  8},
	{   314814805984497,	18,	 1,	14,	  8},
	{   317757010459900,	19,	 1,	13,	  9},
	{   320754706859589,	17,	 1,	15,	  7},
	{   323809534311295,	16,	 1,	14,	  7},
	{   326923072338104,	16,	 1,	12,	  8},
	{   330097079277039,	19,	 1,	10,	 11},
	{   333333343267441,	19,	 1,	14,	  8},
	{   336633652448654,	21,	 1,	12,	 10},
	{   340000003576279,	18,	 1,	15,	  7},
	{   343434333801270,	17,	 1,	14,	  7},
	{   346938788890839,	16,	 1,	13,	  7},
	{   350515455007553,	20,	 1,	14,	  8},
	{   354166656732559,	16,	 1,	15,	  6},
	{   357894748449326,	19,	 1,	15,	  7},
	{   361702114343643,	18,	 1,	14,	  7},
	{   365591406822205,	17,	 1,	13,	  7},
	{   369565218687057,	19,	 1,	11,	  9},
	{   373626381158829,	17,	 1,	15,	  6},
	{   377777785062790,	16,	 1,	14,	  6},
	{   382022470235825,	19,	 1,	14,	  7},
	{   386363625526428,	18,	 1,	13,	  7},
	{   390804588794708,	21,	 1,	15,	  7},
	{   395348846912384,	18,	 1,	15,	  6},
	{   400000005960464,	17,	 1,	14,	  6},
	{   404761910438538,	16,	 1,	13,	  6},
	{   409638553857803,	19,	 1,	13,	  7},
	{   414634138345718,	19,	 1,	15,	  6},
	{   419753074645996,	18,	 1,	14,	  6},
	{   425000011920929,	16,	 1,	15,	  5},
	{   430379748344421,	17,	 1,	13,	  6},
	{   435897439718246,	16,	 1,	12,	  6},
	{   441558450460434,	19,	 1,	14,	  6},
	{   447368413209915,	17,	 1,	15,	  5},
	{   453333348035812,	16,	 1,	14,	  5},
	{   459459453821182,	17,	 1,	12,	  6},
	{   465753436088562,	20,	 1,	14,	  6},
	{   472222208976746,	18,	 1,	15,	  5},
	{   478873252868652,	17,	 1,	14,	  5},
	{   485714286565781,	16,	 1,	13,	  5},
	{   492753624916077,	18,	 1,	10,	  7},
	{   500000000000000,	19,	 1,	15,	  5},
	{   507462680339813,	18,	 1,	14,	  5},
	{   515151500701903,	17,	 1,	13,	  5},
	{   523076951503754,	16,	 1,	12,	  5},
	{   531250000000000,	16,	 1,	15,	  4},
	{   539682567119597,	18,	 1,	13,	  5},
	{   548387110233307,	21,	 1,	15,	  5},
	{   552631556987762,	20,	 1,	 3,	 19},
	{   557377040386199,	17,	 1,	12,	  5},
	{   566666662693024,	16,	 1,	14,	  4},
	{   571428596973419,	19,	 1,	13,	  5},
	{   576271176338196,	18,	 1,	10,	  6},
	{   580645143985748,	18,	 1,	12,	  5},
	{   586206912994385,	21,	 1,	14,	  5},
	{   591549277305603,	23,	 1,	 8,	  9},
	{   596491217613220,	17,	 1,	14,	  4},
	{   607142865657806,	16,	 1,	13,	  4},
	{   612903237342834,	19,	 1,	12,	  5},
	{   618181824684143,	16,	 1,	10,	  5},
	{   622950792312622,	19,	 1,	15,	  4},
	{   629629611968994,	18,	 1,	14,	  4},
	{   634920656681061,	20,	 1,	10,	  6},
	{   641509413719177,	17,	 1,	13,	  4},
	{   647058844566345,	21,	 1,	 3,	 17},
	{   653846144676208,	16,	 1,	12,	  4},
	{   666666686534882,	19,	 1,	14,	  4},
	{   680000007152557,	18,	 1,	13,	  4},
	{   688524603843689,	21,	 1,	15,	  4},
	{   693877577781677,	17,	 1,	12,	  4},
	{   701754391193390,	20,	 1,	14,	  4},
	{   708333313465118,	16,	 1,	15,	  3},
	{   716981112957001,	19,	 1,	13,	  4},
	{   723404228687286,	17,	 1,	 9,	  5},
	{   730769217014313,	18,	 1,	12,	  4},
	{   739130437374115,	23,	 1,	12,	  5},
	{   745098054409027,	20,	 1,	13,	  4},
	{   755555570125580,	16,	 1,	14,	  3},
	{   765957474708557,	19,	 1,	12,	  4},
	{   772727251052856,	16,	 1,	10,	  4},
	{   782608687877655,	21,	 1,	13,	  4},
	{   790697693824768,	18,	 1,	15,	  3},
	{   800000011920929,	17,	 1,	14,	  3},
	{   809523820877075,	16,	 1,	13,	  3},
	{   818181812763214,	17,	 1,	10,	  4},
	{   829268276691437,	19,	 1,	15,	  3},
	{   837209284305573,	20,	 1,	 9,	  5},
	{   850000023841858,	18,	 1,	14,	  3},
	{   857142865657806,	17,	 1,	13,	  3},
	{   863636374473572,	18,	 1,	10,	  4},
	{   871794879436493,	16,	 1,	12,	  3},
	{   883720934391022,	19,	 1,	14,	  3},
	{   894736826419830,	17,	 1,	 9,	  4},
	{   904761910438538,	18,	 1,	13,	  3},
	{   918918907642365,	17,	 1,	12,	  3},
	{   926829278469086,	20,	 1,	14,	  3},
	{   936170220375061,	22,	 1,	 6,	  7},
	{   944444417953491,	16,	 1,	11,	  3},
	{   952380955219269,	19,	 1,	13,	  3},
	{   959999978542328,	23,	 1,	 9,	  5},
	{   971428573131561,	18,	 1,	12,	  3},
	{   978723406791687,	23,	 1,	 6,	  7},
	{  1000000000000000,	17,	 1,	11,	  3},
	{  1022222280502318,	22,	 1,	14,	  3},
	{  1030303001403809,	16,	 1,	10,	  3},
	{  1043478250503540,	23,	 1,	 1,	 23},
	{  1052631616592407,	21,	 1,	13,	  3},
	{  1062500000000000,	16,	 1,	15,	  2},
	{  1076923131942749,	20,	 1,	12,	  3},
	{  1085714340209961,	23,	 1,	10,	  4},
	{  1096774220466614,	17,	 1,	10,	  3},
	{  1105263113975524,	20,	 1,	 1,	 19},
	{  1117647051811218,	19,	 1,	11,	  3},
	{  1133333325386047,	16,	 1,	14,	  2},
	{  1142857193946838,	23,	 1,	13,	  3},
	{  1151515126228333,	18,	 1,	10,	  3},
	{  1161290287971497,	20,	 1,	11,	  3},
	{  1172413825988770,	22,	 1,	12,	  3},
	{  1187500000000000,	18,	 1,	15,	  2},
	{  1200000047683716,	17,	 1,	14,	  2},
	{  1214285731315613,	16,	 1,	13,	  2},
	{  1225806474685669,	23,	 1,	12,	  3},
	{  1241379261016846,	19,	 1,	15,	  2},
	{  1259259223937988,	18,	 1,	14,	  2},
	{  1272727251052856,	20,	 1,	10,	  3},
	{  1285714268684387,	17,	 1,	13,	  2},
	{  1297297239303588,	23,	 1,	 0,	 37},
	{  1307692289352417,	16,	 1,	12,	  2},
	{  1333333373069763,	19,	 1,	14,	  2},
	{  1360000014305115,	18,	 1,	13,	  2},
	{  1371428608894348,	23,	 1,	 6,	  5},
	{  1384615421295166,	17,	 1,	12,	  2},
	{  1399999976158142,	20,	 1,	14,	  2},
	{  1416666626930237,	16,	 1,	11,	  2},
	{  1428571462631226,	19,	 1,	13,	  2},
	{  1440000057220459,	17,	 1,	 4,	  5},
	{  1461538434028625,	18,	 1,	12,	  2},
	{  1478260874748230,	19,	 1,	 8,	  3},
	{  1500000000000000,	17,	 1,	11,	  2},
	{  1519999980926514,	18,	 1,	 4,	  5},
	{  1533333301544189,	22,	 1,	14,	  2},
	{  1545454502105713,	16,	 1,	10,	  2},
	{  1565217375755310,	21,	 1,	13,	  2},
	{  1583333373069763,	18,	 1,	11,	  2},
	{  1600000023841858,	23,	 1,	14,	  2},
	{  1619047641754150,	20,	 1,	12,	  2},
	{  1636363625526428,	17,	 1,	10,	  2},
	{  1652173876762390,	18,	 1,	 0,	 23},
	{  1666666626930237,	19,	 1,	11,	  2},
	{  1679999947547913,	21,	 1,	12,	  2},
	{  1700000047683716,	16,	 1,	 9,	  2},
	{  1714285731315613,	23,	 1,	13,	  2},
	{  1727272748947144,	18,	 1,	10,	  2},
	{  1750000000000000,	20,	 1,	11,	  2},
	{  1769230723381042,	22,	 1,	12,	  2},
	{  1789473652839661,	17,	 1,	 9,	  2},
	{  1809523820877075,	19,	 1,	10,	  2},
	{  1826086997985839,	21,	 1,	11,	  2},
	{  1840000033378601,	23,	 1,	12,	  2},
	{  1888888835906982,	16,	 1,	 8,	  2},
	{  1904761910438538,	20,	 1,	10,	  2},
	{  1919999957084656,	23,	 1,	 4,	  5},
	{  2000000000000000,	17,	 1,	 8,	  2},
	{  2086956501007080,	23,	 1,	 0,	 23},
	{  2105263233184814,	20,	 1,	 9,	  2},
	{  2125000000000000,	16,	 1,	15,	  1},
	{  2190476179122924,	23,	 1,	10,	  2},
	{  2210526227951049,	21,	 1,	 9,	  2},
	{  2235294103622437,	19,	 1,	 8,	  2},
	{  2266666650772095,	16,	 1,	14,	  1},
	{  2285714387893677,	23,	 1,	 6,	  3},
	{  2315789461135864,	22,	 1,	 9,	  2},
	{  2333333253860474,	20,	 1,	 8,	  2},
	{  2352941274642944,	19,	 1,	 0,	 17},
	{  2375000000000000,	18,	 1,	15,	  1},
	{  2400000095367432,	17,	 1,	14,	  1},
	{  2428571462631226,	16,	 1,	13,	  1},
	{  2470588207244873,	20,	 1,	 0,	 17},
	{  2500000000000000,	19,	 1,	15,	  1},
	{  2533333301544189,	18,	 1,	14,	  1},
	{  2571428537368774,	17,	 1,	13,	  1},
	{  2615384578704834,	16,	 1,	12,	  1},
	{  2666666746139526,	19,	 1,	14,	  1},
	{  2714285612106323,	18,	 1,	13,	  1},
	{  2769230842590332,	17,	 1,	12,	  1},
	{  2799999952316284,	20,	 1,	14,	  1},
	{  2833333253860474,	16,	 1,	11,	  1},
	{  2857142925262451,	19,	 1,	13,	  1},
	{  2923076868057251,	18,	 1,	12,	  1},
	{  3000000000000000,	17,	 1,	11,	  1},
	{  3066666603088379,	22,	 1,	14,	  1},
	{  3090909004211426,	16,	 1,	10,	  1},
	{  3142857074737549,	21,	 1,	13,	  1},
	{  3166666746139526,	18,	 1,	11,	  1},
	{  3200000047683716,	23,	 1,	14,	  1},
	{  3230769157409668,	20,	 1,	12,	  1},
	{  3272727251052856,	17,	 1,	10,	  1},
	{  3333333253860474,	19,	 1,	11,	  1},
	{  3400000095367432,	16,	 1,	 9,	  1},
	{  3428571462631226,	23,	 1,	13,	  1},
	{  3454545497894287,	18,	 1,	10,	  1},
	{  3500000000000000,	20,	 1,	11,	  1},
	{  3538461446762085,	22,	 1,	12,	  1},
	{  3599999904632568,	17,	 1,	 9,	  1},
	{  3636363744735718,	19,	 1,	10,	  1},
	{  3666666746139526,	21,	 1,	11,	  1},
	{  3777777671813965,	16,	 1,	 8,	  1},
	{  3818181753158569,	20,	 1,	10,	  1},
	{  4000000000000000,	17,	 1,	 8,	  1},
	{  4199999809265136,	20,	 1,	 9,	  1},
	{  4250000000000000,	16,	 1,	 7,	  1},
	{  4363636493682861,	23,	 1,	10,	  1},
	{  4400000095367431,	21,	 1,	 9,	  1},
	{  4444444656372070,	19,	 1,	 8,	  1},
	{  4500000000000000,	17,	 1,	 7,	  1},
	{  4599999904632568,	22,	 1,	 9,	  1},
	{  4666666507720947,	20,	 1,	 8,	  1},
	{  4750000000000000,	18,	 1,	 7,	  1},
	{  4800000190734863,	23,	 1,	 9,	  1},
	{  4857142925262451,	16,	 1,	 6,	  1},
	{  5000000000000000,	19,	 1,	 7,	  1},
	{  5142857074737549,	17,	 1,	 6,	  1},
	{  5250000000000000,	20,	 1,	 7,	  1},
	{  5333333492279053,	23,	 1,	 8,	  1},
	{  5428571224212646,	18,	 1,	 6,	  1},
	{  5500000000000000,	21,	 1,	 7,	  1},
	{  5666666507720947,	16,	 1,	 5,	  1},
	{  5714285850524902,	19,	 1,	 6,	  1},
	{  6000000000000000,	17,	 1,	 5,	  1},
	{  6285714149475098,	21,	 1,	 6,	  1},
	{  6333333492279053,	18,	 1,	 5,	  1},
	{  6571428775787354,	22,	 1,	 6,	  1},
	{  6666666507720947,	19,	 1,	 5,	  1},
	{  6800000190734863,	16,	 1,	 4,	  1},
	{  6857142925262451,	23,	 1,	 6,	  1},
	{  7000000000000000,	20,	 1,	 5,	  1},
	{  7199999809265137,	17,	 1,	 4,	  1},
	{  7333333492279053,	21,	 1,	 5,	  1},
	{  7599999904632568,	18,	 1,	 4,	  1},
	{  7666666507720947,	22,	 1,	 5,	  1},
	{  8000000000000000,	19,	 1,	 4,	  1},
	{  8399999618530273,	20,	 1,	 4,	  1},
	{  8500000000000000,	16,	 1,	 3,	  1},
	{  8800000190734863,	21,	 1,	 4,	  1},
	{  9000000000000000,	17,	 1,	 3,	  1},
	{  9199999809265136,	22,	 1,	 4,	  1},
	{  9500000000000000,	18,	 1,	 3,	  1},
	{  9600000381469726,	23,	 1,	 4,	  1},
	{ 10000000000000000,	19,	 1,	 3,	  1},
	{ 10500000000000000,	20,	 1,	 3,	  1},
	{ 11000000000000000,	21,	 1,	 3,	  1},
	{ 11333333015441894,	16,	 1,	 2,	  1},
	{ 11500000000000000,	22,	 1,	 3,	  1},
	{ 12000000000000000,	17,	 1,	 2,	  1},
	{ 12666666984558106,	18,	 1,	 2,	  1},
	{ 13333333015441894,	19,	 1,	 2,	  1},
	{ 14000000000000000,	20,	 1,	 2,	  1},
	{ 14666666984558106,	21,	 1,	 2,	  1},
	{ 15333333015441894,	22,	 1,	 2,	  1},
	{ 16000000000000000,	23,	 1,	 2,	  1},
	{ 17000000000000000,	16,	 1,	 1,	  1},
	{ 18000000000000000,	17,	 1,	 1,	  1},
	{ 19000000000000000,	18,	 1,	 1,	  1},
	{ 20000000000000000,	19,	 1,	 1,	  1},
	{ 21000000000000000,	20,	 1,	 1,	  1},
	{ 22000000000000000,	21,	 1,	 1,	  1},
	{ 23000000000000000,	22,	 1,	 1,	  1},
	{ 24000000000000000,	23,	 1,	 1,	  1},
};
EXPORT_SYMBOL(ambarella_rct_pll_table);

u32 ambarella_rct_find_pll_table_index(unsigned long rate, u32 pre_scaler,
	const struct pll_table_s *p_table, u32 table_size)
{
	u64 divident;
	u64 divider;
	u32 start;
	u32 middle;
	u32 end;
	u32 index_limit;
	u64 diff = 0;
	u64 diff_low = 0xFFFFFFFFFFFFFFFF;
	u64 diff_high = 0xFFFFFFFFFFFFFFFF;

	pr_debug("pre_scaler = [0x%08X]\n", pre_scaler);

	divident = rate;
	divident *= pre_scaler;
	divident *= (1000 * 1000 * 1000);
	divider = (REF_CLK_FREQ / (1000 * 1000));
	AMBCLK_DO_DIV(divident, divider);

	index_limit = (table_size - 1);
	start = 0;
	end = index_limit;
	middle = table_size / 2;
	while (p_table[middle].multiplier != divident) {
		if (p_table[middle].multiplier < divident) {
			start = middle;
		} else {
			end = middle;
		}
		middle = (start + end) / 2;
		if (middle == start || middle == end) {
			break;
		}
	}
	pr_debug("divident = [%llu]\n", divident);
	if ((middle > 0) && ((middle + 1) <= index_limit)) {
		if (p_table[middle - 1].multiplier < divident) {
			diff_low = (divident -
				p_table[middle - 1].multiplier);
		} else {
			diff_low = (p_table[middle - 1].multiplier -
				divident);
		}
		if (p_table[middle].multiplier < divident) {
			diff = (divident - p_table[middle].multiplier);
		} else {
			diff = (p_table[middle].multiplier - divident);
		}
		if (p_table[middle + 1].multiplier < divident) {
			diff_high = (divident -
				p_table[middle + 1].multiplier);
		} else {
			diff_high = (p_table[middle + 1].multiplier -
				divident);
		}
		pr_debug("multiplier[%u] = [%llu]\n", (middle - 1),
			p_table[middle - 1].multiplier);
		pr_debug("multiplier[%u] = [%llu]\n", (middle),
			p_table[middle].multiplier);
		pr_debug("multiplier[%u] = [%llu]\n", (middle + 1),
			p_table[middle + 1].multiplier);
	} else if ((middle == 0) && ((middle + 1) <= index_limit)) {
		if (p_table[middle].multiplier < divident) {
			diff = (divident - p_table[middle].multiplier);
		} else {
			diff = (p_table[middle].multiplier - divident);
		}
		if (p_table[middle + 1].multiplier < divident) {
			diff_high = (divident -
				p_table[middle + 1].multiplier);
		} else {
			diff_high = (p_table[middle + 1].multiplier -
				divident);
		}
		pr_debug("multiplier[%u] = [%llu]\n", (middle),
			p_table[middle].multiplier);
		pr_debug("multiplier[%u] = [%llu]\n", (middle + 1),
			p_table[middle + 1].multiplier);
	} else if ((middle > 0) && ((middle + 1) > index_limit)) {
		if (p_table[middle - 1].multiplier < divident) {
			diff_low = (divident -
				p_table[middle - 1].multiplier);
		} else {
			diff_low = (p_table[middle - 1].multiplier -
				divident);
		}
		if (p_table[middle].multiplier < divident) {
			diff = (divident - p_table[middle].multiplier);
		} else {
			diff = (p_table[middle].multiplier - divident);
		}
		pr_debug("multiplier[%u] = [%llu]\n", (middle - 1),
			p_table[middle - 1].multiplier);
		pr_debug("multiplier[%u] = [%llu]\n", (middle),
			p_table[middle].multiplier);
	}
	pr_debug("diff_low = [%llu]\n", diff_low);
	pr_debug("diff = [%llu]\n", diff);
	pr_debug("diff_high = [%llu]\n", diff_high);
	if (diff_low < diff) {
		if (middle) {
			middle--;
		}
	}
	if (diff_high < diff) {
		middle++;
		if (middle > index_limit) {
			middle = index_limit;
		}
	}
	pr_debug("middle = [%u]\n", middle);

	return middle;
}
EXPORT_SYMBOL(ambarella_rct_find_pll_table_index);

/* ==========================================================================*/
unsigned long ambarella_rct_clk_get_rate(struct clk *c)
{
	union ctrl_reg_u ctrl_reg;
	union frac_reg_u frac_reg;
	u32 pre_scaler_reg;
	u32 post_scaler_reg;
	u32 pll_int;
	u32 sdiv;
	u32 sout;
	u64 divident;
	u64 divider;
	u64 frac;

	if (c->ctrl_reg != PLL_REG_UNAVAILABLE) {
		ctrl_reg.w = amba_rct_readl(c->ctrl_reg);
		if ((ctrl_reg.s.power_down == 1) ||
			(ctrl_reg.s.halt_vco == 1)) {
			c->rate = 0;
			goto ambarella_rct_clk_get_rate_exit;
		}
	} else {
		ctrl_reg.w = 0;
	}
	if (c->frac_reg != PLL_REG_UNAVAILABLE) {
		frac_reg.w = amba_rct_readl(c->frac_reg);
	} else {
		frac_reg.w = 0;
	}
	if (c->pres_reg != PLL_REG_UNAVAILABLE) {
		pre_scaler_reg = amba_rct_readl(c->pres_reg);
	} else {
		pre_scaler_reg = 1;
	}
	if (c->post_reg != PLL_REG_UNAVAILABLE) {
		post_scaler_reg = amba_rct_readl(c->post_reg);
	} else {
		post_scaler_reg = 1;
	}

	pll_int = ctrl_reg.s.intp;
	sdiv = ctrl_reg.s.sdiv;
	sout = ctrl_reg.s.sout;

	divident = (REF_CLK_FREQ * (pll_int + 1) * (sdiv + 1));
	divider = (pre_scaler_reg * (sout + 1) * post_scaler_reg);
	if (ctrl_reg.s.frac_mode) {
		if (frac_reg.s.nega) {
			/* Negative */
			frac = (0x80000000 - frac_reg.s.frac);
			frac = (REF_CLK_FREQ * frac * (sdiv + 1));
			frac >>= 32;
			divident = divident - frac;
		} else {
			/* Positive */
			frac = frac_reg.s.frac;
			frac = (REF_CLK_FREQ * frac * (sdiv + 1));
			frac >>= 32;
			divident = divident + frac;
		}
	}

	if (c->extra_scaler > 1) {
		divider *= c->extra_scaler;
	}

	AMBCLK_DO_DIV(divident, divider);
	c->rate = divident;

ambarella_rct_clk_get_rate_exit:
	return c->rate;
}
EXPORT_SYMBOL(ambarella_rct_clk_get_rate);

int ambarella_rct_clk_set_rate(struct clk *c, unsigned long rate)
{
	int ret_val = -1;
	u32 pre_scaler;
	u32 ctrl2;
	u32 ctrl3;
	u64 divident;
	u64 divider;
	u32 middle;
	union ctrl_reg_u ctrl_reg;
	union frac_reg_u frac_reg;
	u64 diff;

	if (c->extra_scaler > 1) {
		rate *= c->extra_scaler;
	}

	if (!rate) {
		ret_val = -1;
		goto ambarella_rct_clk_set_rate_exit;
	}

	if ((c->ctrl_reg == PLL_REG_UNAVAILABLE) &&
		(c->frac_reg == PLL_REG_UNAVAILABLE) &&
		(c->ctrl2_reg == PLL_REG_UNAVAILABLE) &&
		(c->ctrl3_reg == PLL_REG_UNAVAILABLE) &&
		(c->pres_reg == PLL_REG_UNAVAILABLE) &&
		(c->post_reg != PLL_REG_UNAVAILABLE) && c->max_divider) {
		divider = (REF_CLK_FREQ + (rate >> 1) - 1) / rate;
		if (!divider) {
			ret_val = -1;
			goto ambarella_rct_clk_set_rate_exit;
		}
		if (divider > c->max_divider) {
			divider = c->max_divider;
		}
		amba_rct_writel(c->post_reg, divider);
		ret_val = 0;
		goto ambarella_rct_clk_set_rate_exit;
	}

	if ((c->ctrl_reg != PLL_REG_UNAVAILABLE) &&
		(c->post_reg != PLL_REG_UNAVAILABLE)) {
		if (c->pres_reg != PLL_REG_UNAVAILABLE) {
			pre_scaler = amba_rct_readl(c->pres_reg);
		} else {
			pre_scaler = 1;
		}

		middle = ambarella_rct_find_pll_table_index(rate, pre_scaler,
			ambarella_rct_pll_table, AMBARELLA_RCT_PLL_TABLE_SIZE);

		ctrl_reg.w = amba_rct_readl(c->ctrl_reg);
		ctrl_reg.s.intp = ambarella_rct_pll_table[middle].intp;
		ctrl_reg.s.sdiv = ambarella_rct_pll_table[middle].sdiv;
		ctrl_reg.s.sout = ambarella_rct_pll_table[middle].sout;
		ctrl_reg.s.frac_mode = 0;
		ctrl_reg.s.force_lock = 1;
		ctrl_reg.s.write_enable = 0;
		amba_rct_writel(c->ctrl_reg, ctrl_reg.w);
		ctrl_reg.s.write_enable = 1;
		amba_rct_writel(c->ctrl_reg, ctrl_reg.w);
		ctrl_reg.s.write_enable = 0;
		amba_rct_writel(c->ctrl_reg, ctrl_reg.w);

		amba_rct_writel(c->post_reg, ambarella_rct_pll_table[middle].post);

		if (c->frac_mode) {
			c->rate = ambarella_rct_clk_get_rate(c);
			if (c->rate < rate) {
				diff = rate - c->rate;
			} else {
				diff = c->rate - rate;
			}
			divident = (diff * pre_scaler *
				(ambarella_rct_pll_table[middle].sout + 1) *
				ambarella_rct_pll_table[middle].post);
			divident = divident << 32;
			divider = ((u64)REF_CLK_FREQ *
				(ambarella_rct_pll_table[middle].sdiv + 1));
			AMBCLK_DO_DIV_ROUND(divident, divider);
			if (c->rate <= rate) {
				frac_reg.s.nega	= 0;
				frac_reg.s.frac	= divident;
			} else {
				frac_reg.s.nega	= 1;
				frac_reg.s.frac	= 0x80000000 - divident;
			}
			amba_rct_writel(c->frac_reg, frac_reg.w);

			ctrl_reg.w = amba_rct_readl(c->ctrl_reg);
			if (diff) {
				ctrl_reg.s.frac_mode = 1;
			} else {
				ctrl_reg.s.frac_mode = 0;
			}
			ctrl_reg.s.force_lock = 1;
			ctrl_reg.s.write_enable = 1;
			amba_rct_writel(c->ctrl_reg, ctrl_reg.w);

			ctrl_reg.s.write_enable	= 0;
			amba_rct_writel(c->ctrl_reg, ctrl_reg.w);
		}
		if (ctrl_reg.s.frac_mode) {
			ctrl2 = 0x3f770000;
			ctrl3 = 0x00069300;
		} else {
			ctrl2 = 0x3f770000;
			ctrl3 = 0x00068300;
		}

		if (c->ctrl2_reg != PLL_REG_UNAVAILABLE) {
			amba_rct_writel(c->ctrl2_reg, ctrl2);
		}
		if (c->ctrl3_reg != PLL_REG_UNAVAILABLE) {
			amba_rct_writel(c->ctrl3_reg, ctrl3);
		}
	}
	ret_val = 0;

ambarella_rct_clk_set_rate_exit:
	c->rate = ambarella_rct_clk_get_rate(c);
	return ret_val;
}
EXPORT_SYMBOL(ambarella_rct_clk_set_rate);

int ambarella_rct_clk_enable(struct clk *c)
{
	union ctrl_reg_u ctrl_reg;

	if (c->ctrl_reg == PLL_REG_UNAVAILABLE) {
		return -1;
	}

	ctrl_reg.w = amba_rct_readl(c->ctrl_reg);
	ctrl_reg.s.power_down = 0;
	ctrl_reg.s.halt_vco = 0;
	ctrl_reg.s.write_enable = 1;
	amba_rct_writel(c->ctrl_reg, ctrl_reg.w);

	ctrl_reg.s.write_enable	= 0;
	amba_rct_writel(c->ctrl_reg, ctrl_reg.w);

	c->rate = ambarella_rct_clk_get_rate(c);

	return 0;
}
EXPORT_SYMBOL(ambarella_rct_clk_enable);

int ambarella_rct_clk_disable(struct clk *c)
{
	union ctrl_reg_u ctrl_reg;

	if (c->ctrl_reg == PLL_REG_UNAVAILABLE) {
		return -1;
	}

	ctrl_reg.w = amba_rct_readl(c->ctrl_reg);
	ctrl_reg.s.power_down = 1;
	ctrl_reg.s.halt_vco = 1;
	ctrl_reg.s.write_enable = 1;
	amba_rct_writel(c->ctrl_reg, ctrl_reg.w);

	ctrl_reg.s.write_enable	= 0;
	amba_rct_writel(c->ctrl_reg, ctrl_reg.w);

	c->rate = ambarella_rct_clk_get_rate(c);

	return 0;
}
EXPORT_SYMBOL(ambarella_rct_clk_disable);

struct clk_ops ambarella_rct_pll_ops = {
	.enable		= ambarella_rct_clk_enable,
	.disable	= ambarella_rct_clk_disable,
	.get_rate	= ambarella_rct_clk_get_rate,
	.round_rate	= NULL,
	.set_rate	= ambarella_rct_clk_set_rate,
	.set_parent	= NULL,
};
EXPORT_SYMBOL(ambarella_rct_pll_ops);

/* ==========================================================================*/
unsigned long ambarella_rct_scaler_get_rate(struct clk *c)
{
	u32 divider;

	if (!c->parent || !c->parent->ops || !c->parent->ops->get_rate) {
		return 0;
	}

	if (c->divider) {
		c->rate = c->parent->ops->get_rate(c->parent) / c->divider;
		return c->rate;
	}

	if (c->post_reg != PLL_REG_UNAVAILABLE) {
		divider = amba_rct_readl(c->post_reg);
		if (divider) {
			c->rate = c->parent->ops->get_rate(c->parent) / divider;
			return c->rate;
		}
	}

	return 0;
}
EXPORT_SYMBOL(ambarella_rct_scaler_get_rate);

int ambarella_rct_scaler_set_rate(struct clk *c, unsigned long rate)
{
	u32 divider;

	if (!c->parent || !c->parent->ops || !c->parent->ops->get_rate) {
		return -1;
	}
	if (c->post_reg == PLL_REG_UNAVAILABLE) {
		return -1;
	}
	if (!c->max_divider) {
		return -1;
	}
	if (!rate) {
		return -1;
	}

	divider = ((c->parent->ops->get_rate(c->parent) + (rate >> 2) - 1) /
		rate);
	if (!divider) {
		return -1;
	}
	if (divider > c->max_divider) {
		divider = c->max_divider;
	}

	amba_rct_writel(c->post_reg, divider);
	c->rate = ambarella_rct_scaler_get_rate(c);

	return 0;
}
EXPORT_SYMBOL(ambarella_rct_scaler_set_rate);

struct clk_ops ambarella_rct_scaler_ops = {
	.enable		= NULL,
	.disable	= NULL,
	.get_rate	= ambarella_rct_scaler_get_rate,
	.round_rate	= NULL,
	.set_rate	= ambarella_rct_scaler_set_rate,
	.set_parent	= NULL,
};
EXPORT_SYMBOL(ambarella_rct_scaler_ops);

/* ==========================================================================*/
struct clk *clk_get(struct device *dev, const char *id)
{
	struct clk *p;
	struct clk *clk = ERR_PTR(-ENOENT);

	mutex_lock(&ambarella_clock_mutex);
	list_for_each_entry(p, &ambarella_all_clocks, list) {
		if (strcmp(id, p->name) == 0) {
			clk = p;
			break;
		}
	}
	mutex_unlock(&ambarella_clock_mutex);

	return clk;
}
EXPORT_SYMBOL(clk_get);

void clk_put(struct clk *clk)
{
}
EXPORT_SYMBOL(clk_put);

int clk_enable(struct clk *clk)
{
	if (IS_ERR(clk) || clk == NULL) {
		return -EINVAL;
	}

	clk_enable(clk->parent);

	mutex_lock(&ambarella_clock_mutex);
	if (clk->ops && clk->ops->enable) {
		(clk->ops->enable)(clk);
	}
	mutex_unlock(&ambarella_clock_mutex);

	return 0;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	if (IS_ERR(clk) || clk == NULL) {
		return;
	}

	mutex_lock(&ambarella_clock_mutex);
	if (clk->ops && clk->ops->disable) {
		(clk->ops->disable)(clk);
	}
	mutex_unlock(&ambarella_clock_mutex);

	clk_disable(clk->parent);
}
EXPORT_SYMBOL(clk_disable);

unsigned long clk_get_rate(struct clk *clk)
{
	if (IS_ERR(clk)) {
		return 0;
	}

	if (clk->ops != NULL && clk->ops->get_rate != NULL) {
		return (clk->ops->get_rate)(clk);
	}

	if (clk->parent != NULL) {
		return clk_get_rate(clk->parent);
	}

	return clk->rate;
}
EXPORT_SYMBOL(clk_get_rate);

long clk_round_rate(struct clk *clk, unsigned long rate)
{
	if (!IS_ERR(clk) && clk->ops && clk->ops->round_rate) {
		return (clk->ops->round_rate)(clk, rate);
	}

	return rate;
}
EXPORT_SYMBOL(clk_round_rate);

int clk_set_rate(struct clk *clk, unsigned long rate)
{
	int ret;

	if (IS_ERR(clk)) {
		return -EINVAL;
	}

	if ((clk->ops == NULL) || (clk->ops->set_rate == NULL)) {
		return -EINVAL;
	}

	mutex_lock(&ambarella_clock_mutex);
	ret = (clk->ops->set_rate)(clk, rate);
	mutex_unlock(&ambarella_clock_mutex);

	return ret;
}
EXPORT_SYMBOL(clk_set_rate);

struct clk *clk_get_parent(struct clk *clk)
{
	return clk->parent;
}
EXPORT_SYMBOL(clk_get_parent);

int clk_set_parent(struct clk *clk, struct clk *parent)
{
	int ret = 0;

	if (IS_ERR(clk)) {
		return -EINVAL;
	}

	mutex_lock(&ambarella_clock_mutex);
	if (clk->ops && clk->ops->set_parent) {
		ret = (clk->ops->set_parent)(clk, parent);
	}
	mutex_unlock(&ambarella_clock_mutex);

	return ret;
}
EXPORT_SYMBOL(clk_set_parent);

int ambarella_register_clk(struct clk *clk)
{
	mutex_lock(&ambarella_clock_mutex);
	list_add(&clk->list, &ambarella_all_clocks);
	mutex_unlock(&ambarella_clock_mutex);

	return 0;
}
EXPORT_SYMBOL(ambarella_register_clk);

/* ==========================================================================*/
static struct proc_dir_entry *clock_file = NULL;

static int ambarella_clock_proc_read(char *page, char **start,
	off_t off, int count, int *eof, void *data)
{
	int retlen = 0;
	struct clk *p;

	if (off != 0) {
		retlen = 0;
		goto ambarella_clock_proc_read_exit;
	}

	retlen += scnprintf((page + retlen), (count - retlen),
			"\nClock Information:\n");
	mutex_lock(&ambarella_clock_mutex);
	list_for_each_entry(p, &ambarella_all_clocks, list) {
		if (retlen >= count) {
			break;
		}
		retlen += scnprintf((page + retlen), (count - retlen),
				"\t%s:\t%lu Hz\n",
				p->name, p->ops->get_rate(p));
	}
	mutex_unlock(&ambarella_clock_mutex);
	*eof = 1;

ambarella_clock_proc_read_exit:
	return retlen;
}

static int ambarella_clock_proc_write(struct file *file,
	const char __user *buffer, unsigned long count, void *data)
{
	int ret_val = 0;
	char str[32];
	u32 str_len;
	u32 freq_hz;
	u32 pre_scaler;
	u32 ctrl2;
	u32 ctrl3;
	u64 divident;
	u64 divider;
	u32 middle;
	union ctrl_reg_u ctrl_reg;
	union frac_reg_u frac_reg;
	u64 diff;
	u32 freq_hz_int;

	str_len = (count < sizeof(str)) ? count : sizeof(str);
	memset(str, 0, sizeof(str));
	ret_val = copy_from_user(str, buffer, str_len);
	if (ret_val) {
		pr_err("%s: copy_from_user() = %d\n", __func__, ret_val);
		goto ambarella_clock_proc_write_exit;
	}
	freq_hz = simple_strtoul(str, NULL, 0);
	pre_scaler = 1;

	middle = ambarella_rct_find_pll_table_index(freq_hz, pre_scaler,
		ambarella_rct_pll_table, AMBARELLA_RCT_PLL_TABLE_SIZE);

	ctrl_reg.w = 0;
	ctrl_reg.s.intp = ambarella_rct_pll_table[middle].intp;
	ctrl_reg.s.sdiv = ambarella_rct_pll_table[middle].sdiv;
	ctrl_reg.s.sout = ambarella_rct_pll_table[middle].sout;
	ctrl_reg.s.frac_mode = 0;
	ctrl_reg.s.force_lock = 1;
	ctrl_reg.s.write_enable = 0;

	pr_info("post_scaler = [0x%08X]\n",
		ambarella_rct_pll_table[middle].post);

	divident = (REF_CLK_FREQ * (ctrl_reg.s.intp + 1) *
		(ctrl_reg.s.sdiv + 1));
	divider = (pre_scaler * (ctrl_reg.s.sout + 1) *
		ambarella_rct_pll_table[middle].post);
	AMBCLK_DO_DIV(divident, divider);
	freq_hz_int = divident;
	if (freq_hz_int < freq_hz) {
		diff = freq_hz - freq_hz_int;
	} else {
		diff = freq_hz_int - freq_hz;
	}
	divident = (diff * pre_scaler *
		(ambarella_rct_pll_table[middle].sout + 1) *
		ambarella_rct_pll_table[middle].post);
	divident = divident << 32;
	divider = ((u64)REF_CLK_FREQ *
		(ambarella_rct_pll_table[middle].sdiv + 1));
	AMBCLK_DO_DIV_ROUND(divident, divider);
	if (freq_hz_int <= freq_hz) {
		frac_reg.s.nega	= 0;
		frac_reg.s.frac	= divident;
	} else {
		frac_reg.s.nega	= 1;
		frac_reg.s.frac	= 0x80000000 - divident;
	}
	pr_info("frac_reg = [0x%08X]\n", frac_reg.w);

	if (diff) {
		ctrl_reg.s.frac_mode = 1;
	} else {
		ctrl_reg.s.frac_mode = 0;
	}
	ctrl_reg.s.force_lock = 1;
	ctrl_reg.s.write_enable	= 0;
	pr_info("ctrl_reg = [0x%08X]\n", ctrl_reg.w);

	if (ctrl_reg.s.frac_mode) {
		ctrl2 = 0x3f770000;
		ctrl3 = 0x00069300;
	} else {
		ctrl2 = 0x3f770000;
		ctrl3 = 0x00068300;
	}
	pr_info("ctrl2_reg = [0x%08X]\n", ctrl2);
	pr_info("ctrl3_reg = [0x%08X]\n", ctrl3);

	ret_val = count;

ambarella_clock_proc_write_exit:
	return ret_val;
}

int __init ambarella_init_pll(void)
{
	int ret_val = 0;

	clock_file = create_proc_entry("clock", S_IRUGO,
		get_ambarella_proc_dir());
	if (clock_file == NULL) {
		ret_val = -ENOMEM;
		pr_err("%s: create proc file (clock) fail!\n", __func__);
		goto ambarella_init_pll_exit;
	} else {
		clock_file->read_proc = ambarella_clock_proc_read;
		clock_file->write_proc = ambarella_clock_proc_write;
	}

ambarella_init_pll_exit:
	return ret_val;
}

