% 
% Advent of Code 2022 - Day 12.
%

:- use_module(library(lists)).
:- use_module(library(clpfd)).
:- working_directory(_, 'C:/Users/stephen.mccoy/github/aoc2022/day12/').

:- dynamic field/1.
:- dynamic goal/1.

:- set_prolog_flag(stack_limit, 2_147_483_648).


% Translate S and E to codes for the field data.
fvtranslate(83, 96) :- !.		% S is the start.
fvtranslate(69, 123) :- !.		% E is the end.
fvtranslate(C, C).				% Otherwise use the same code.

% Reading and storing the input file data.
store_field_row([]).
store_field_row([X | Tail]) :-
	assertz(field(X)),
	!,
	store_field_row(Tail).

read_input_line(Stream, RowNum) :- 
	read_line_to_string(Stream, String),
	String \= end_of_file,
	string_codes(String, Codes1),
	% Translate char to elevation: 97 is 'a', 122 is 'z'
	maplist(fvtranslate, Codes1, Codes2),
	findall(RowNum/Col/Value, nth1(Col, Codes2, Value), Row),
	store_field_row(Row).

read_input_lines(Stream, RowNum) :-
	read_input_line(Stream, RowNum),
	!,
	N is RowNum + 1,
	read_input_lines(Stream, N).
read_input_lines(_,_).

read_input_data(FileName) :-
	retractall(field(_)),
	open(FileName, read, Stream),
	read_input_lines(Stream, 1),
	close(Stream).

% Neighbours on the grid.
neighbours(R1/C/H1, R2/C/H2, south) :-
	R2 #= R1 + 1,
	field(R1/C/H1),
	field(R2/C/H2).
neighbours(R/C1/H1, R/C2/H2, west) :-
	C2 #= C1 + 1,
	field(R/C1/H1),
	field(R/C2/H2).
neighbours(R1/C/H1, R2/C/H2, north) :-
	R2 #= R1 - 1,
	field(R1/C/H1),
	field(R2/C/H2).
neighbours(R/C1/H1, R/C2/H2, east) :-
	C2 #= C1 - 1,
	field(R/C1/H1),
	field(R/C2/H2).

step_up(R1/C1/H1, R2/C2/H2) :-
	neighbours(R1/C1/H1, R2/C2/H2, _),
	H2 #= H1 + 1.

% Best first search set up.

s(R1/C1/H, R2/C2/H, 1) :-
	neighbours(R1/C1/H, R2/C2/H, _).

s(N1, N2, 1) :-
	step_up(N1, N2).

h(R/C/_, Distance) :-
	goal(GR/GC/_),
	DR is abs(GR - R),
	DC is abs(GC - C),
	Distance is (DR + DC).

% Goal is this one location.
goal(R/C/123) :-
	field(R/C/123).

solve(Node, Solution) :-
	track([], Node, Solution).

% Depth first search version.
track(Path, Node, [Node | Path]) :-
	goal(Node).
track(Path, R1/C1/H1, Sol) :-
	neighbours(R1/C1/H1, R2/C2/H2, _),
	DH #= H2 - H1,
	member(DH, [0,1]),
	not(member(R2/C2/H2, Path)), 
	track([R1/C1/H1 | Path], R2/C2/H2, Sol).

% path(From, Path, Cost)
/*
path(G, [G], 0) :-
	goal(G), !.
path(R1/C1/H1, Path, Cost) :-
	step_up(R2/C2/H1, R3/C3/H3),
	plateau(R1/C1/H1, R2/C2/H1, Track, Cost1),
	path(R3/C3/H3, Tail, Cost2),
	append(Track, Tail, Path),
	Cost is Cost1 + Cost2.
*/

legal_step(R1/C1/H1, R2/C2/H2) :-
	neighbours(R1/C1/H1, R2/C2/H2, _),
	DH #= H2 - H1,
	member(DH, [0,1]).

plateau(Node, Node, [Node]).
plateau(R1/C1/H, Rn/Cn/H, [Rn/Cn/H | Path]) :-
	path(R1/C1/H, R2/C2/H, Path),
	neighbours(R2/C2/H, Rn/Cn/H, _),
	not(member(Rn/Cn/H, Path)).

path(Node, Node, [Node]).
path(FirstNode, LastNode, [LastNode | Path]) :-
	path(FirstNode, OneButLast, Path),
	neighbours(OneButLast, LastNode, _),
	legal_step(OneButLast, LastNode),
	not(member(LastNode, Path)).

depth_first_iterative_deepening(Node, Solution) :-
	path(Node, GoalNode, Solution),
	goal(GoalNode).

solve_for(FileName, Solution, Cost) :-
	read_input_data(FileName),
	Start = _/_/96,
	bestfirst(Start, Solution),
%	depth_first_iterative_deepening(Start, Solution),
	length(Solution, Cost).

day12_part1 :-
	writeln('Advent of Code 2022.\nDay 12, Part 1.'),
	writeln('Test run...'),
	solve_for('test12.txt', Solution1, Cost1),
	Steps1 is Cost1 - 1,
	format('Solution is: ~w', [Solution1]),
	format('Number of steps = ~w\n', [Steps1]).
/*	
	writeln('Full run...'),
	solve_for('day12.txt', Solution2, Cost2),
	Steps2 is Cost2 - 1,
	format('Solution is: ~w', [Solution2]),
	format('Number of steps = ~w\n', [Steps2]).
*/
