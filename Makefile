build:
	gcc -o tema2 main.c arb_suf.c arb_suf_comp.c -Wall -Werror -Wextra -g

clean:
	rm tema2