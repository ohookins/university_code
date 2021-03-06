% ------------------------------------------ %
% Parking.pl solution
%
% Oliver Hookins, SID: 30704553
% Login: s3070455
% ------------------------------------------ %

solve(X) :- print('Not implemented').

%parking_order(placement(identity(T1,N1,1),
%			identity(T2,N2,2),
%			identity(T3,N3,3),
%			identity(T4,N4,4),
%			identity(T5,N5,5),
%			identity(T6,N6,6))).

identity('Accountant',_,Z1) :-
	location(_,centre,Z1).


/*	identity(_,'D P Timms',Z2),
	even(Z2),
	identity('Managing Director',_,Z3),
	even(Z3),
	identity('Sales Director',Y1,_),
	family(simms,Y1),
	identity('Managing Director',Y2,_),
	identity('Accountant',Y3,_),
	family(F1,Y2),
	family(F1,Y3),
	identity(_,Y4,Z4),
	identity(_,Y5,Z5),
	family(F2,Y4),
	family(F3,Y5),
	opposite(Y4,Y5),
	Z4 \= Z5.
*/
	
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

