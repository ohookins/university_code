% ------------------------------------------ %
% Parking.pl solution
%
% Oliver Hookins, SID: 30704553
% Login: s3070455
% ------------------------------------------ %

solve(X) :-
	parking_order(X).
	
parking_order(placement(identity(T1,N1,P1),
			identity(T2,N2,P2),
			identity(T3,N3,P3),
			identity(T4,N4,P4),
			identity(T5,N5,P5),
			identity(T6,N6,P6))).

identity('Accountant',_,X) :-
	location(_,centre,X).

identity(_,'D P Timms',X) :-
	even(X).
	
identity('Managing Director',_,X) :-
	even(X).

identity('Sales Director',X,_) :-
	family(simms,X).

identity('Managing Director',X1,_) :-
	identity('Accountant',X2,_),
	family(Z,X1),
	family(Z,X2).

identity(_,X1,Y1) :-
	identity(_,X2,Y2),
	family(Z1,X1),
	family(Z2,X2),
	opposite(Y1,Y2),
	Z1 \= Z2.

location(top,left,1).
location(top,centre,2).
location(top,right,3).
location(bottom,left,4).
location(bottom,centre,5).
location(bottom,right,6).

odd(1).
odd(3).
odd(5).

even(2).
even(4).
even(6).

opposite(1,4).
opposite(2,5).
opposite(3,6).

family(timms,'D P Timms').
family(timms,'L G Timms').
family(timms,'R N Timms').
family(simms,'A E Simms').
family(simms,'C F Simms').
family(simms,'T J Simms').

