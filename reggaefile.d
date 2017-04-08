import reggae;

enum CompileCommand {
	Compile = "g++ -c -std=c++14 -O0 -ggdb -Wall -Werror -pedantic -fdiagnostics-color=always $in -o $out",
	Link = "g++ -std=c++14 -O3 -ggdb -Wall -Werror -pedantic -fdiagnostics-color=always -lm -ldl -lSDL2 -lSDL2_image -lSDL2_mixer -lGL -lassimp $in -o $out",
}

Target[] MakeObjects() {
	import std.file : dirEntries, SpanMode;
	import std.process : executeShell;
	import std.algorithm : map;
	import std.array : array, replace, split;
	import std.range : chain;

	Target[] objs;

	foreach (f; chain(dirEntries("src/", "*.cpp", SpanMode.breadth), dirEntries("src/", "*.c", SpanMode.breadth)).filter!(x => !x.isDir)) {
		auto exec = executeShell("g++ -MM " ~ f);
		if (exec.status) {
			import std.stdio;
			stderr.writeln("Returned: ", exec.status, "\n", exec.output);
			assert(0);
		}

		auto head = exec.output.split(":")[1].replace("\n", " ").split(" ").filter!(s => !s.empty && s != "\\").map!(x => Target(x)).array;
		objs ~= Target(f ~ ".o", CompileCommand.Compile, [Target(f)], head);
	}

	return objs;
}

Build myBuild() {
	auto objs = MakeObjects();

	auto raycast = Target("trigonoverthrow", CompileCommand.Link, objs);

	return Build(raycast);
}
