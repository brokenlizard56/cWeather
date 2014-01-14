all:
	gcc -o cWeather cWeather.c -lcurl -ljson -std=gnu99
