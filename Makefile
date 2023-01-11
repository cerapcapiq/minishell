

exec = minishell.out

src = lexer.c ast.c main.c parser.c token.c visitor.c

obj = $(src:.c=.o)

flag = -g -lreadline

$(exec) : $(obj)
	gcc $(obj) $(flag) -o $(exec)

%.o: %.c %.h
	gcc -c $(flag) $< -o $@

clean: 
	-rm *.out 
	-rm *.o
	-rm src/*.o
