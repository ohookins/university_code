before(am(H1,M1),pm(H2,M2)).
before(am(12,M1),am(H2,M2)) :-
	H2 < 12.
before(am(H1,M1),am(H2,M2)) :-
	H1 < H2, H2 < 12.
before(am(H,M1),am(H,M2)) :-
	M1 < M2.
before(pm(12,M1),pm(H2,M2)) :-
	H2 < 12.
before(pm(H1,M1),pm(H2,M2)) :-
	H1 < H2, H2 < 12.
before(pm(H,M1),pm(H,M2)) :-
	M1 < M2.

