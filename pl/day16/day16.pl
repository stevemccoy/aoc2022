%
% Advent of Code 2022 - Day 16, Proboscidea Volcanium
%

:- working_directory(_, 'c:/users/stephen.mccoy/github/aoc2022/pl/day16/').

:- dynamic valve/3.

% DCG for input file.

identifier_code(C) --> [C],
	{	code_type(C, alpha)
	}.

identifier_codes([HC | TC]) --> identifier_code(HC), identifier_codes(TC).
identifier_codes([C]) --> identifier_code(C).

vname(Name) --> identifier_codes(Codes),
	{	string_codes(Name, Codes)
	}.

digit(D) --> [D],
	{	code_type(D, digit)
	}.

digits([D | Tail]) --> digit(D), !, digits(Tail).
digits([]) --> [].

flow(F) --> digits(Digits),
	{	number_codes(F, Digits)
	}.

valve_list([V1 | Tail]) --> vname(V1), ", ", valve_list(Tail).
valve_list([V1]) --> vname(V1).

optional_s --> "s ", !.
optional_s --> " ".

tunnel_spec(VL) --> "tunnels lead to valves ", valve_list(VL).
tunnel_spec([VN]) --> "tunnel leads to valve ", vname(VN).

valve_spec(VN, F, VL) --> "Valve ", vname(VN), " has flow rate=", flow(F), 
	"; ", tunnel_spec(VL),
	{	assertz(valve(VN, F, VL))
	}.

% Read the input file.

read_input_line(Stream) :- 
	read_line_to_string(Stream, String),
	string_codes(String, Chars),
	phrase(valve_spec(_, _, _), Chars, []).

read_input_lines(Stream) :-
	read_input_line(Stream),
	!,
	read_input_lines(Stream).
read_input_lines(_).

read_input_data(FileName) :-
	retractall(valve(_,_,_)),
	open(FileName, read, Stream),
	read_input_lines(Stream),
	close(Stream).

% Solve the problem.
% state(at(VN), remaining(T), score(S), opened(VL))
% solve(Time, PlanBefore/ScoreBefore, PlanAfter/ScoreAfter) :-

depthfirst()


path(V1, V2, 1, [mv(V2)]) :-
	valve(V1, _, VL),
	member(V2, VL).
path(V1, VN, N, Plan) :-
	valve(V1, _, VL),
	member(V2, VL),
	path(V2, VN, M, Tail),
	N is M + 1,
	not(member(mv(V2), Plan)),
	append(Tail, [mv(V2)], Plan).

% Open valve.
move(
	state(at(V1), remaining(T1), score(S1), opened(VL1), plan(P1)), 
	state(at(V1), remaining(T2), score(S2), opened([V1 | VL1]), plan([op(V1) | P1]))) :-

	T1 >= 1,
	T2 is T1 - 1,
	valve(V1, F, _),
	not(member(V1, VL1)),
	Inc is F * T2,
	S2 is S1 + Inc.

% Move from one valve to another.
move(
	state(at(V1), remaining(T1), score(S1), opened(VL1), plan(P1)), 
	state(at(V2), remaining(T2), score(S1), opened(VL1), plan([mv(V2) | P1]))) :-
	
	T1 >= 1,
	T2 is T1 - 1,
	valve(V1, _, Conns),
	member(V2, Conns).

move(
	state(at(V1), remaining(T1), score(S1), opened(VL1), plan(P1)), 
	state(at(V2), remaining(T2), score(S1), opened(VL1), plan(P2))) :-

	T1 >= 1,
	
	% Find a valve worth moving to.
	valve(V2, F, _),
	V1 \== V2,
	F > 0,
	not(member(V2, VL1)),
	path(V1, V2, DT, Moves),

	% Move to V2.

	T2 is T1 - DT,
	append(Moves, P1, P2).




% Maintain an agenda of partially expanded plans in decreasing order of score.

get_score(state(_, _, score(Sc1), _, _), Sc1).

best_score([state(_, _, score(Sc1), _, _) | _ ], Sc1).
best_score([], 0).

insert(State, States, [State | States]) :-
	get_score(State, Sc1),
	best_score(States, Sc2),
	Sc1 >= Sc2, !.
insert(State, [S1 | States1], [S1 | States2]) :-
	insert(State, States1, States2).

% Merge given list of states into the ordered agenda given.
merge([], L, L).
merge([S | Tail], L1, L2) :-
	insert(S, L1, L3),
	merge(Tail, L3, L2).

% solve(FromStates, Plan, Score).

solve([state(at(_), remaining(0), score(Score), opened(_), plan(Plan)) | _], Plan, Score).
solve([S1 | Rest1], Plan, Score) :-
	findall(S2, (move(S1, S2)), SL2),
	merge(SL2, Rest1, Rest2),
	solve(Rest2, Plan, Score).

% TODO: Find paths between valves which have a non-zero capacity.


solve_day16 :-
	read_input_data('test16.txt'),
	Start = state(at("AA"), remaining(30), score(0), opened([]), plan([])),
	solve([Start], Plan, Score),
	format('Plan found with score ~w:\n~w\n', [Score, Plan]).
