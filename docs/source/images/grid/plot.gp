# p
reset
{
  set terminal epslatex color standalone header '\usepackage{amsmath}' size 13,12 font ',17'
  set output 'staggered1.tex'
  unset border
  set noxtics
  set noytics
  set size ratio -1.
  set xrange [-1.5:11.5]
  set yrange [-1.:12.]
  set linestyle 1 lw 10 lc rgb '#000000'
  set style arrow 1 nohead ls 1
# shadow
  set object rectangle from first 1,1 to first 10,10 fc rgb '#00FFFF' fillstyle solid border lc rgb '#00FFFF' back
  set object rectangle from first 0,0 to first 1,11 fc rgb '#AAAAAA' fillstyle solid border lc rgb '#AAAAAA' back
  set object rectangle from first 10,0 to first 11,11 fc rgb '#AAAAAA' fillstyle solid border lc rgb '#AAAAAA' back
# 9 3x3 grids
  do for [y0=0:8:4] {
    do for [x0=0:8:4] {
      do for [i=0:3] {
        set arrow from first x0+i,y0+0 to first x0+i,y0+3 as 1
      }
      do for [j=0:3] {
        set arrow from first x0+0,y0+j to first x0+3,y0+j as 1
      }
      do for [j=0:2] {
        do for [i=0:2] {
          if(x0 == 0 && i == 0){
            set object circle at first x0+1.0+i,y0+0.5+j size first 0.05 fill solid fc rgb '#000000'
          }else{
            if(x0 == 8 && i == 2){
              set object circle at first x0+0.0+i,y0+0.5+j size first 0.05 fill solid fc rgb '#000000'
            }else{
              set object circle at first x0+0.5+i,y0+0.5+j size first 0.05 fill solid fc rgb '#000000'
            }
          }
        }
      }
    }
  }
# index labels
# x
  set label '$0$'      at first  0.5, -0.75 center
  set label '$1$'      at first  1.5, -0.75 center
  set label '$2$'      at first  2.5, -0.75 center
  set label '$i-1$'    at first  4.5, -0.75 center
  set label '$i$'      at first  5.5, -0.75 center
  set label '$i+1$'    at first  6.5, -0.75 center
  set label '$itot-1$' at first  8.5, -0.75 center
  set label '$itot$'   at first  9.5, -0.75 center
  set label '$itot+1$' at first 10.5, -0.75 center
# y
  set label '$0$'      at first -1.0,  0.5 center
  set label '$1$'      at first -1.0,  1.5 center
  set label '$2$'      at first -1.0,  2.5 center
  set label '$j-1$'    at first -1.0,  4.5 center
  set label '$j$'      at first -1.0,  5.5 center
  set label '$j+1$'    at first -1.0,  6.5 center
  set label '$jtot-1$' at first -1.0,  8.5 center
  set label '$jtot  $' at first -1.0,  9.5 center
  set label '$jtot+1$' at first -1.0, 10.5 center
  plot \
    NaN notitle
}

# ux
reset
{
  set terminal epslatex color standalone header '\usepackage{amsmath}' size 13,12 font ',17'
  set output 'staggered2.tex'
  unset border
  set noxtics
  set noytics
  set size ratio -1.
  set xrange [-1.5:11.5]
  set yrange [-1.:12.]
  set linestyle 1 lw 10 lc rgb '#000000'
  set style arrow 1 nohead ls 1
# shadow
  set object rectangle from first 1,1 to first 10,10 fc rgb '#00FFFF' fillstyle solid border lc rgb '#00FFFF' back
  set object rectangle from first 0,0 to first 1,11 fc rgb '#AAAAAA' fillstyle solid border lc rgb '#AAAAAA' back
  set object rectangle from first 10,0 to first 11,11 fc rgb '#AAAAAA' fillstyle solid border lc rgb '#AAAAAA' back
# 9 3x3 grids
  do for [y0=0:8:4] {
    do for [x0=0:8:4] {
      do for [i=0:3] {
        set arrow from first x0+i,y0+0 to first x0+i,y0+3 as 1
      }
      do for [j=0:3] {
        set arrow from first x0+0,y0+j to first x0+3,y0+j as 1
      }
    }
  }
# velocity arrows
  arrlngt = 0.25
  set style line 2 lc rgb '#FF0000' lw 10
  set style arrow 2 head size 0.2,10 filled front ls 2
# ux
  do for [y0=1:11] {
    if (y0 != 4 && y0 != 8) {
      do for [x0=1:10] {
        set arrow from first x0-arrlngt,y0-0.5 to first x0+arrlngt,y0-0.5 as 2
      }
    }
  }
# index labels
# x
  set label '$0$'      at first  1.0, -0.75 center
  set label '$1$'      at first  2.0, -0.75 center
  set label '$2$'      at first  3.0, -0.75 center
  set label '$i-2$'    at first  4.0, -0.75 center
  set label '$i-1$'    at first  5.0, -0.75 center
  set label '$i  $'    at first  6.0, -0.75 center
  set label '$i+1$'    at first  7.0, -0.75 center
  set label '$itot-2$' at first  8.0, -0.75 center
  set label '$itot-1$' at first  9.0, -0.75 center
  set label '$itot  $' at first 10.0, -0.75 center
# y
  set label '$0$'      at first -1.0,  0.5 center
  set label '$1$'      at first -1.0,  1.5 center
  set label '$2$'      at first -1.0,  2.5 center
  set label '$j-1$'    at first -1.0,  4.5 center
  set label '$j$'      at first -1.0,  5.5 center
  set label '$j+1$'    at first -1.0,  6.5 center
  set label '$jtot-1$' at first -1.0,  8.5 center
  set label '$jtot  $' at first -1.0,  9.5 center
  set label '$jtot+1$' at first -1.0, 10.5 center
  plot \
    NaN notitle
}

# uy
reset
{
  set terminal epslatex color standalone header '\usepackage{amsmath}' size 13,12 font ',17'
  set output 'staggered3.tex'
  unset border
  set noxtics
  set noytics
  set size ratio -1.
  set xrange [-1.5:11.5]
  set yrange [-1.:12.]
  set linestyle 1 lw 10 lc rgb '#000000'
  set style arrow 1 nohead ls 1
# shadow
  set object rectangle from first 1,1 to first 10,10 fc rgb '#00FFFF' fillstyle solid border lc rgb '#00FFFF' back
  set object rectangle from first 0,0 to first 1,11 fc rgb '#AAAAAA' fillstyle solid border lc rgb '#AAAAAA' back
  set object rectangle from first 10,0 to first 11,11 fc rgb '#AAAAAA' fillstyle solid border lc rgb '#AAAAAA' back
# 9 3x3 grids
  do for [y0=0:8:4] {
    do for [x0=0:8:4] {
      do for [i=0:3] {
        set arrow from first x0+i,y0+0 to first x0+i,y0+3 as 1
      }
      do for [j=0:3] {
        set arrow from first x0+0,y0+j to first x0+3,y0+j as 1
      }
    }
  }
# velocity arrows
  arrlngt = 0.25
  set style line 2 lc rgb '#0000FF' lw 10
  set style arrow 2 head size 0.2,10 filled front ls 2
# uy
  do for [y0=0:10] {
    do for [x0=0:10] {
      if (x0 != 3 && x0 != 7) {
        if(x0 == 0){
          set arrow from first x0+1.0,y0-arrlngt to first x0+1.0,y0+arrlngt as 2
        }else{
          if(x0 == 10){
            set arrow from first x0+0.0,y0-arrlngt to first x0+0.0,y0+arrlngt as 2
          }else{
            set arrow from first x0+0.5,y0-arrlngt to first x0+0.5,y0+arrlngt as 2
          }
        }
      }
    }
  }
# index labels
# x
  set label '$0$'      at first  0.5, -0.75 center
  set label '$1$'      at first  1.5, -0.75 center
  set label '$2$'      at first  2.5, -0.75 center
  set label '$i-1$'    at first  4.5, -0.75 center
  set label '$i$'      at first  5.5, -0.75 center
  set label '$i+1$'    at first  6.5, -0.75 center
  set label '$itot-1$' at first  8.5, -0.75 center
  set label '$itot$'   at first  9.5, -0.75 center
  set label '$itot+1$' at first 10.5, -0.75 center
# y
  set label '$0$'      at first -1.0,  0.0 center
  set label '$1$'      at first -1.0,  1.0 center
  set label '$2$'      at first -1.0,  2.0 center
  set label '$3$'      at first -1.0,  3.0 center
  set label '$j-1$'    at first -1.0,  4.0 center
  set label '$j$'      at first -1.0,  5.0 center
  set label '$j+1$'    at first -1.0,  6.0 center
  set label '$j+2$'    at first -1.0,  7.0 center
  set label '$jtot-1$' at first -1.0,  8.0 center
  set label '$jtot  $' at first -1.0,  9.0 center
  set label '$jtot+1$' at first -1.0, 10.0 center
  plot \
    NaN notitle
}

# others
reset
{
  set terminal epslatex color standalone header '\usepackage{amsmath}' size 13,12 font ',17'
  set output 'staggered4.tex'
  unset border
  set noxtics
  set noytics
  set size ratio -1.
  set xrange [-1.5:11.5]
  set yrange [-1.:12.]
  set linestyle 1 lw 10 lc rgb '#000000'
  set style arrow 1 nohead ls 1
# shadow
  set object rectangle from first 1,1 to first 10,10 fc rgb '#00FFFF' fillstyle solid border lc rgb '#00FFFF' back
  set object rectangle from first 0,0 to first 1,11 fc rgb '#AAAAAA' fillstyle solid border lc rgb '#AAAAAA' back
  set object rectangle from first 10,0 to first 11,11 fc rgb '#AAAAAA' fillstyle solid border lc rgb '#AAAAAA' back
# 9 3x3 grids
  do for [y0=0:8:4] {
    do for [x0=0:8:4] {
      do for [i=0:3] {
        set arrow from first x0+i,y0+0 to first x0+i,y0+3 as 1
      }
      do for [j=0:3] {
        set arrow from first x0+0,y0+j to first x0+3,y0+j as 1
      }
    }
  }
# corner markers
  do for [y0=0:10] {
    do for [x0=1:10] {
      set object circle at first x0,y0 size first 0.05 fill solid fc rgb '#000000' front
    }
  }
# index labels
# x
  set label '$0$'      at first  1.0, -0.75 center
  set label '$1$'      at first  2.0, -0.75 center
  set label '$2$'      at first  3.0, -0.75 center
  set label '$i-2$'    at first  4.0, -0.75 center
  set label '$i-1$'    at first  5.0, -0.75 center
  set label '$i  $'    at first  6.0, -0.75 center
  set label '$i+1$'    at first  7.0, -0.75 center
  set label '$itot-2$' at first  8.0, -0.75 center
  set label '$itot-1$' at first  9.0, -0.75 center
  set label '$itot  $' at first 10.0, -0.75 center
# y
  set label '$0$'      at first -1.0,  0.0 center
  set label '$1$'      at first -1.0,  1.0 center
  set label '$2$'      at first -1.0,  2.0 center
  set label '$3$'      at first -1.0,  3.0 center
  set label '$j-1$'    at first -1.0,  4.0 center
  set label '$j$'      at first -1.0,  5.0 center
  set label '$j+1$'    at first -1.0,  6.0 center
  set label '$j+2$'    at first -1.0,  7.0 center
  set label '$jtot-1$' at first -1.0,  8.0 center
  set label '$jtot  $' at first -1.0,  9.0 center
  set label '$jtot+1$' at first -1.0, 10.0 center
  plot \
    NaN notitle
}

