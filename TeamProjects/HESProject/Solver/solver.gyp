{
	"includes": [
		"common.gypi"
	],
	"targets": [
		{
			"target_name": "MathParser",
			"product_name": "MathParser",
			"type": "shared_library",
			"sources": [
				"include/MathParser/math_parser.h",

				"lib/MathParser/type_aliases.h",
				"lib/MathParser/eval_stack.h",
				"lib/MathParser/eval_table.h",
				"lib/MathParser/Parser.h",
				"lib/MathParser/Scanner.h",

				"lib/MathParser/eval_stack.cpp",
				"lib/MathParser/eval_table.cpp",
				"lib/MathParser/math_parser.cpp",
				"lib/MathParser/Parser.cpp",
				"lib/MathParser/Parser_user.cpp",
				"lib/MathParser/Scanner.cpp",
			],
			"include_dirs": [
				"include/MathParser"
			],
			"direct_dependent_settings": {
				"include_dirs": [ "include/MathParser/" ]
			}
		},
		{
			"target_name": "ExplicitSolver",
			"type": "shared_library",
			"sources": [
				# explicit solver sources
			],
			"include_dirs": [
				"include/MathParser"
			],
			"dependencies": [
				"MathParser"
			]
		},
		{
			"target_name": "ImplicitSolver",
			"type": "shared_library",
			"sources": [
				# explicit solver sources
			],
			"include_dirs": [
				"include/MathParser"
			],
			"dependencies": [
				"MathParser"
			]
		}
	]
}
