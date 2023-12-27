FROM gcc:latest
WORKDIR /app
COPY . /app
ENV CFLAGS=-O2 -std=c99
RUN cc main.c asprintf.c -o main
CMD ["./main"]
