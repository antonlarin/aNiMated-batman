{
	"includes": [
		"common.gypi"
	],
	"targets": [
		{
			"target_name": "MathParser",
			"cflags_cc!": [ "-fno-exceptions" ],
			"cflags": [
				"-Wswitch-enum",
				"-std=c++11",
				"-fexceptions",
				"-fPIC"
			],
			"product_name": "MathParser",
			"type": "shared_library",
			"sources": [
				"include/MathParser/math_parser.h",

				"lib/MathParser/type_aliases.h",
				"lib/MathParser/eval_stack.h",
				"lib/MathParser/eval_table.h",
				"lib/MathParser/Parser.h",
				"lib/MathParser/Scanner.h",
				"lib/MathParser/InternalFunction.h",

				"lib/MathParser/eval_stack.cpp",
				"lib/MathParser/eval_table.cpp",
				"lib/MathParser/math_parser.cpp",
				"lib/MathParser/Parser.cpp",
				"lib/MathParser/Parser_user.cpp",
				"lib/MathParser/Scanner.cpp",
				"lib/MathParser/InternalFunction.cpp"
			],
			"include_dirs": [
				"include/MathParser"
			]
		},
		{
			"target_name": "ImplicitSolver",
			"cflags": [
				"-Wswitch-enum",
				"-std=c++11",
				"-fexceptions",
				"-fPIC"
			],
			"type": "shared_library",
			"sources": [
				"src/CommonSolver/HE_Integrator.h",
				"src/CommonSolver/HE_Task.h",
				"src/CommonSolver/HES_Solver.h",

				"src/CommonSolver/HE_Task.cpp",
				"src/CommonSolver/HES_Solver.cpp",

				"src/ImplicitSolver/HE_Integrator.cpp"
			],
			"include_dirs": [
				"include/MathParser",
				"src/CommonSolver"
			],
			"dependencies": [
				"MathParser"
			]
		},
		{
			"target_name": "ExplicitSolver",
			"cflags": [
				"-Wswitch-enum",
				"-std=c++11",
				"-fexceptions",
				"-fPIC"
			],
			"type": "shared_library",
			"sources": [
				"src/CommonSolver/HE_Integrator.h",
				"src/CommonSolver/HE_Task.h",
				"src/CommonSolver/HES_Solver.h",

				"src/CommonSolver/HE_Task.cpp",
				"src/CommonSolver/HES_Solver.cpp",

				"src/ExplicitSolver/HE_Integrator.cpp"
			],
			"include_dirs": [
				"include/MathParser",
				"src/CommonSolver"
			],
			"dependencies": [
				"MathParser"
			]
		}
	]
}
