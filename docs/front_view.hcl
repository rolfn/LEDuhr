unitlength 1.5;           # Maßstab 1.5:1
font Sans_Serif 3;

set wd2 80; # half width of the plate
set ht2 50; # half height of the plate

set wd2DP1L  60;     # half width of display board 1  # 4.72" = 119.9mm ~= 120mm
set ht2DP1L  25.4;   # half height of display board 1 # 1" = 25.4mm
set wd2DP1   60;     # half width of Display 1
set ht2DP1   20.4;   # half height of Display 1
set DP1posOffs -4;

set wd2DP2L  25;   # half width of display board 2  # 1.97" = 50.038mm
set ht2DP2L  14.1; # half of display board 2 # 1.11" = 28.194mm
set wd2DP2   25;   # half width of Display 2
set ht2DP2   9.5;  # half height of Display 2
set DP2posOffs -10;

set indRad   2.5;
set DP1Rad   1.5;
set DP2Rad   0.67;
set DP1Lwd   0.15;
set DP2Lwd   0.1;

set dimDist  3.5;

set DP1LholeRad 1;   # ø0.07874" = ø2mm
set DP1LholeDistX 112.6;
set DP1LoffsX [expr (2 * $wd2DP1L - $DP1LholeDistX) / 2];
set DP1LholeDistY 45.7;
set DP1LoffsY [expr (2 * $ht2DP1L - $DP1LholeDistY) / 2];

set DP2LholeRad 1;   # ø0.07874" = ø2mm
set DP2LholeDistX 43.7;
set DP2LoffsX [expr (2 * $wd2DP2L - $DP2LholeDistX) / 2];
set DP2LholeDistY 22.9;
set DP2LoffsY [expr (2 * $ht2DP2L - $DP2LholeDistY) / 2];

set thickLine 0.65;
set midLine 0.25;
set thinLine 0.125;

# upper left position
set UL "-$wd2  -$ht2";
set board1UL "[expr $DP1posOffs - $wd2DP1] [expr -$ht2DP1L - $ht2DP2L]";
# upper left position of display board 1
set dp1UL "[X $board1UL] [expr [Y $board1UL] + $ht2DP1L - $ht2DP1]";

#########moveto [expr $DP1posOffs - $wd2DP1L] [expr -$ht2DP1L - $ht2DP2L + $ht2DP2 /2 ];
set DP1pos $dp1UL;

# upper left position of display 1
set board2UL "0 [expr [Y $board1UL] + 2 * $ht2DP1L]";
# upper left position of display board 2
set dp2UL "[X $board2UL] [expr [Y $board2UL] + $ht2DP2L - $ht2DP2]";
# upper left position of display 2
set indicatorPos "[expr [X $board1UL] + 24] [expr [Y $board2UL] + $ht2DP2L]";

# Indikator
moveto $indicatorPos;
set tmp "[expr [X $board1UL] + 2 * $wd2DP1L + 4 * $dimDist] [Y [here]]";
pen lack dashdotted 0.2;
lineto [expr [X $board2UL] - 3 * $dimDist] [Y [here]];
lineto [expr [X [here]] + 2 * $dimDist] [Y [here]] [X $tmp] [Y [here]];

moveto $indicatorPos;
pen orange; fillcircle $indRad;
pen black $midLine solid; circle $indRad;
moveto -$wd2 [expr [Y [here]] + $indRad + $dimDist];
dimline [X $indicatorPos] [Y [here]];
moveto [expr [X $indicatorPos] - $indRad - $dimDist] [expr [Y $indicatorPos] - $indRad];
dimlinerel 0 [expr 2 * $indRad];
moveto [X $tmp] -$ht2;
dimline "[X [here]] [Y $tmp]";

proc drawBig7Seg {pos} {
  proc drawSeg {x} {
    pen orange;
    global DP1Lwd;
    fillpolygon $x;
    pen black $DP1Lwd solid;
    polygon $x;
  }
  proc draw7Seg {xoffs} {
    offset $xoffs 0;
    # "A"
    drawSeg {6.03 5.71 7.99 8.12 19.10 8.12 21.99 5.63 21.51 5.06 6.66 5.03};
    # "B"
    drawSeg {22.27 6.05 19.39 8.46 17.56 18.61 18.81 20.02 20.57 18.61 22.77 6.58};
    # "C"
    drawSeg {18.74 20.75 16.82 22.32 15.17 32.28 17.11 34.67 17.82 34.20 19.94 22.27};
    # "D"
    drawSeg {0.76 35.11 1.10 35.61 16.04 35.64 16.77 35.09 14.75 32.65 3.44 32.73};
    # "E"
    drawSeg {3.88 20.75 2.10 22.27 0.00 34.14 0.42 34.72 3.38 32.10 5.11 22.2};
    # "F"
    drawSeg {5.53 6.03 4.85 6.58 2.75 18.61 3.96 19.99 5.76 18.45 7.63 8.46};
    # "G"
    drawSeg {4.30 20.41 5.50 21.91 16.61 21.88 18.34 20.39 17.16 18.89 6.03 18.89};

    offset [expr -$xoffs] 0; # reset offset;
  }

  global DP1Rad DP1Lwd;

  pen black $DP1Lwd solid;

  moveto $pos;   # upper left corner Display 2
  moverel 8.7 13.6; circle $DP1Rad;
  moverel -2.54 14.96; circle $DP1Rad;
  moverel 88.4 -22; circle $DP1Rad;

  moveto $pos;
  moverel 65.22 13.6;
  pen orange; fillcircle $DP1Rad;
  pen black; circle $DP1Rad;
  moverel -2.54 14.96;
  pen orange; fillcircle $DP1Rad;
  pen black; circle $DP1Rad;

  moveto $pos;

  offset $pos;
  set x [X $pos];
  set y [Y $pos];

  set d {12.63 37.63 68.23 93.23}; # local offsets for each 7-Seg

  foreach i $d {
    draw7Seg $i;
  }

  offset [expr -1 * $x] [expr -1 * $y]; # reset global offset;

}

proc drawSmall7Seg {pos} {
  proc drawSeg {x} {
    pen orange;
    global DP2Lwd;
    fillpolygon $x;
    pen black $DP2Lwd solid;
    polygon $x;
  }
  proc draw7Seg {xoffs} {
    offset $xoffs 0;
    # "A"
    drawSeg {2.43 2.92 3.07 2.39 8.82 2.39 9.25 2.91 8.31 3.74 3.14 3.74};
    # "B"
    drawSeg {9.40 3.10 9.86 3.62 9.14 8.66 8.37 9.35 7.77 8.70 8.46 3.90};
    # "C"
    drawSeg {8.31 9.64 7.55 10.31 6.86 15.10 7.59 15.92 8.18 15.42 8.91 10.30};
    # "D"
    drawSeg {0.60 16.08 1.07 16.57 6.82 16.61 7.40 16.07 6.70 15.25 1.52 15.25};
    # "E"
    drawSeg {1.50 9.65 0.69 10.31 0.00 15.38 0.46 15.90 1.42 15.07 2.07 10.30};
    # "F"
    drawSeg {2.28 3.08 1.66 3.61 0.94 8.69 1.52 9.34 2.32 8.66 3.00 3.88};
    # "G"
    drawSeg {1.66 9.50 2.24 10.17 7.41 10.16 8.20 9.48 7.62 8.82 2.45 8.82};

    offset [expr -$xoffs] 0; # reset offset;
  }

  global DP2Rad DP2Lwd;
  pen black $DP2Lwd solid;

  moveto $pos;   # upper left corner Display 2

  moverel 25.63 5.63;  circle $DP2Rad;
  moverel -1.15 7.92;  circle $DP2Rad;
  moverel -14.06 2.52; circle $DP2Rad;
  moverel 25.53 0; circle $DP2Rad;
  moverel 12.7 0;
  pen orange; fillcircle $DP2Rad;
  pen black; circle $DP2Rad;
  moverel -25.4 0;
  pen orange; fillcircle $DP2Rad;
  pen black; circle $DP2Rad;

  moveto $pos;

  offset $pos;
  set x [X $pos];
  set y [Y $pos];

  set d {1.04 13.74 26.44 39.14}; # local offsets for each 7-Seg

  foreach i $d {
    draw7Seg $i;
  }

  offset [expr -1 * $x] [expr -1 * $y]; # reset global offset;

}

# whole front plate
moveto $UL;
pen black $thickLine solid;
rectangle [expr 2 * $wd2] [expr 2 * $ht2];
pen black $midLine solid;
moverel 0 -$dimDist;
dimlinerel [expr 2 * $wd2] 0;
moverel $dimDist $dimDist;
dimlinerel 0 [expr 2 * $ht2];

# board Display 1
moveto $board1UL;
pen black $thickLine solid;
rectangle [expr 2 * $wd2DP1L] [expr 2 * $ht2DP1L];
pen black $midLine solid;
moverel [expr -2 * $dimDist] 0;
dimlinerel 0 [expr 2 * $ht2DP1L];
moveto -$wd2 [expr [Y [here]] - 2 * $ht2DP1L - 2 * $dimDist];
dimline "[X $board1UL] [Y [here]]";
dimlinerel [expr 2 * $wd2DP1L] 0;
moveto -$wd2 [expr [Y $board1UL] - $dimDist];
dimline "[expr [X $board1UL] + $DP1LoffsX] [Y [here]]";
dimlinerel $DP1LholeDistX 0;
moverel 0 [expr $dimDist + $DP1LoffsY]; circle $DP1LholeRad;
moverel 0 $DP1LholeDistY; circle $DP1LholeRad;
moverel -$DP1LholeDistX 0; circle $DP1LholeRad;
moverel 0 -$DP1LholeDistY; circle $DP1LholeRad;
moverel [expr -$DP1LoffsX -$dimDist] 0;
dimlinerel 0 $DP1LholeDistY;

moveto [expr [X $board1UL] + 2 * $wd2DP1L + $dimDist] -$ht2;
dimline "[X [here]] [expr [Y $board1UL] + $DP1LoffsY]";

# Display 1
moveto $dp1UL;
pen lightgray;
fillrectangle [expr 2 * $wd2DP1] [expr 2 * $ht2DP1];
pen black $thickLine solid;
rectangle     [expr 2 * $wd2DP1] [expr 2 * $ht2DP1];
pen black $midLine solid;
moveto [expr [X [here]] + 2 * $wd2DP1 + 3 * $dimDist] -$ht2;
dimline "[X [here]] [Y $dp1UL]";
dimlinerel 0 [expr 2 * $ht2DP1];
drawBig7Seg $dp1UL;

# board Display 2
moveto $board2UL;
pen black $thickLine solid;
rectangle [expr 2 * $wd2DP2L] [expr 2 * $ht2DP2L];
pen black $midLine solid;
moverel [expr -2 * $dimDist] 0;
dimlinerel 0 [expr 2 * $ht2DP2L];
moveto -$wd2 [expr [Y [here]] + 2 * $dimDist];
dimline "[X $board2UL] [Y [here]]";
dimlinerel [expr 2 * $wd2DP2L] 0;
moveto -$wd2 [expr [Y $board2UL] + 2 * $ht2DP2L + $dimDist];
dimline "[expr [X $board2UL] + $DP2LoffsX] [Y [here]]";
dimlinerel $DP2LholeDistX 0;
moverel 0 [expr -$dimDist - $DP2LoffsY]; circle $DP2LholeRad;
moverel 0 -$DP2LholeDistY; circle $DP2LholeRad;
moverel -$DP2LholeDistX 0; circle $DP2LholeRad;
moverel 0 $DP2LholeDistY; circle $DP2LholeRad;
moverel [expr -$DP2LoffsX -$dimDist] 0;
dimlinerel 0 -$DP2LholeDistY;

moveto [expr [X $board1UL] + 2 * $wd2DP1L + 2 * $dimDist] -$ht2;
dimline "[X [here]] [expr [Y $board2UL] + $DP2LoffsY]";

# Display 2
moveto $dp2UL;
pen lightgray;
fillrectangle [expr 2 * $wd2DP2] [expr 2 * $ht2DP2];
pen black $thickLine solid;
rectangle     [expr 2 * $wd2DP2] [expr 2 * $ht2DP2];
pen black $midLine solid;
moveto [expr [X $board1UL] + 2 * $wd2DP1L + 5 * $dimDist] -$ht2;
dimline "[X [here]] [expr [Y $dp2UL] + 2 * $ht2DP2]";
dimlinerel 0 [expr -2 * $ht2DP2];

drawSmall7Seg $dp2UL;



