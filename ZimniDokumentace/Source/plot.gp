set terminal pngcairo dashed size 1600,1600 enhanced font "Arial,20"

set xlabel 'x(t)'
set ylabel 'y(t)'

set key top right
set size ratio 1

set xrange [-2:2]
set yrange [-2:2]
set output 'explicitEuler.png'
plot \
'analyticSol.txt'		using ($2):($3) w lp t 'analytické øešení'	pi 5   pt 1 ps 2.5 lw 4	lt rgb "red"		,\
'explicitEuler1.txt'	using ($2):($3) w lp t 'dt=0.5'				pi 1   pt 2 ps 2.5 lw 4	lt rgb "cyan"		,\
'explicitEuler2.txt'	using ($2):($3) w lp t 'dt=0.1'				pi 5   pt 3 ps 2.5 lw 4	lt rgb "blue"		, \
'explicitEuler3.txt'	using ($2):($3) w lp t 'dt=0.05'			pi 10  pt 6 ps 2.5 lw 4	lt rgb "orange"		,\
'explicitEuler4.txt'	using ($2):($3) w lp t 'dt=0.01'			pi 50  pt 9 ps 2.5 lw 4	lt rgb "green"		;

set xrange [-1.2:1.2]
set yrange [-1.2:1.2]
set output 'implicitEuler.png'
plot \
'analyticSol.txt'		using ($2):($3) w lp t 'analytické øešení'	pi 5  pt 1 ps 2.5 lw 4 lt rgb "red" ,\
'implicitEuler1.txt'	using ($2):($3) w lp t 'dt=0.5'				pi 1  pt 2 ps 2.5 lw 4 lt rgb "cyan" ,\
'implicitEuler2.txt'	using ($2):($3) w lp t 'dt=0.1'				pi 5  pt 3 ps 2.5 lw 4 lt rgb "blue", \
'implicitEuler3.txt'	using ($2):($3) w lp t 'dt=0.05'			pi 10 pt 6 ps 2.5 lw 4 lt rgb "orange" ,\
'implicitEuler4.txt'	using ($2):($3) w lp t 'dt=0.01'			pi 50 pt 9 ps 2.5 lw 4 lt rgb "green";

set xrange [-1.2:1.2]
set yrange [-1.2:1.2]
set output 'semiImplEuler.png'
plot \
'analyticSol.txt'		using ($2):($3) w lp t 'analytické øešení'	pi 5  pt 1 ps 2.5 lw 4 lt rgb "red" ,\
'semiImplEuler1.txt'	using ($2):($3) w lp t 'dt=0.5'				pi 1  pt 2 ps 2.5 lw 4 lt rgb "cyan" ,\
'semiImplEuler2.txt'	using ($2):($3) w lp t 'dt=0.25'				pi 5  pt 3 ps 2.5 lw 4 lt rgb "blue", \
'semiImplEuler3.txt'	using ($2):($3) w lp t 'dt=0.1'			pi 10 pt 6 ps 2.5 lw 4 lt rgb "orange" ,\
'semiImplEuler4.txt'	using ($2):($3) w lp t 'dt=0.01'			pi 50 pt 9 ps 2.5 lw 4 lt rgb "green";

set xrange [-1.2:1.2]
set yrange [-1.2:1.2]
set output 'semiImplEulerStab.png'
plot \
'analyticSol.txt'			using ($2):($3) w l t 'analytické øešení'	lw 4 lt rgb "red" ,\
'semiImplEulerStab1.txt'	using ($2):($3) w l t 'dt=0.5'				lw 4 lt rgb "cyan" ,\
'semiImplEulerStab3.txt'	using ($2):($3) w l t 'dt=0.1'				lw 4 lt rgb "orange" ,\
'semiImplEulerStab4.txt'	using ($2):($3) w l t 'dt=0.01'				lw 4 lt rgb "green";

set xrange [-1.2:1.2]
set yrange [-1.2:1.2]
set output 'RK4.png'
plot \
'analyticSol.txt'		using ($2):($3) w lp t 'analytické øešení'	pi 5  pt 1 ps 2.5 lw 4 lt rgb "red" ,\
'RK41.txt'				using ($2):($3) w lp t 'dt=1.0'				pi 1  pt 2 ps 2.5 lw 4 lt rgb "cyan" ,\
'RK42.txt'				using ($2):($3) w lp t 'dt=0.5'				pi 5  pt 3 ps 2.5 lw 4 lt rgb "blue", \
'RK43.txt'				using ($2):($3) w lp t 'dt=0.1'				pi 10 pt 6 ps 2.5 lw 4 lt rgb "orange" ,\
'RK44.txt'				using ($2):($3) w lp t 'dt=0.01'			pi 50 pt 9 ps 2.5 lw 4 lt rgb "green";

set xrange [-1.2:1.2]
set yrange [-1.2:1.2]
set output 'RK4Stab.png'
plot \
'analyticSol.txt'			using ($2):($3) w l t 'analytické øešení'	lw 4 lt rgb "red" ,\
'RK41Stab.txt'	using ($2):($3) w l t 'dt=1.0'				lw 4 lt rgb "cyan" ,\
'RK42Stab.txt'	using ($2):($3) w l t 'dt=0.5'				lw 4 lt rgb "blue",\
'RK43Stab.txt'	using ($2):($3) w l t 'dt=0.1'				lw 4 lt rgb "orange";

set output
set terminal window