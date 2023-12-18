file(GLOB sources app/*.cpp generator/*.cpp lexer/*.c parser/*.cpp utils/*.cpp) 
file(GLOB headers app/*.h generator/*.h lexer/*.h parser/*.h utils/*.h) 

add_executable(cpp2python
	${sources} 
	${headers}
)