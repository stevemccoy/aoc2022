%
% Advent of Code 2022 - Day 19, Not Enough Minerals.
%

:- working_directory(_, 'c:/users/stephen.mccoy/github/aoc2022/pl/day19/').

:- dynamic blueprint/7.

% DCG for input file.

digit(D) --> [D],
	{	code_type(D, digit)
	}.

digits([D | Tail]) --> digit(D), !, digits(Tail).
digits([]) --> [].

integer(F) --> digits(Digits),
	{	number_codes(F, Digits)
	}.

blueprint_spec -->
	"Blueprint ", integer(N), ": ", 
	"Each ore robot costs ", integer(OreOre), " ore. ", 
	"Each clay robot costs ", integer(ClayOre), " ore. ", 
	"Each obsidian robot costs ", integer(ObsOre), " ore and ", integer(ObsClay), " clay. ", 
	"Each geode robot costs ", integer(GeodeOre), " ore and ", integer(GeodeObs), " obsidian.",
	{
		assertz(blueprint(N, OreOre, ClayOre, ObsOre, ObsClay, GeodeOre, GeodeObs))
	}.

% Read the input file.

read_input_line(Stream) :- 
	read_line_to_string(Stream, String),
	string_codes(String, Chars),
	phrase(blueprint_spec, Chars, []).

read_input_lines(Stream) :-
	read_input_line(Stream),
	!,
	read_input_lines(Stream).
read_input_lines(_).

read_input_data(FileName) :-
	retractall(blueprint(_,_,_,_,_,_,_)),
	open(FileName, read, Stream),
	read_input_lines(Stream),
	close(Stream).

% Solve the problem.
