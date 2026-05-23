FROM debian:stable-slim
RUN apt-get update && apt-get install -y gcc && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY main.c .
RUN gcc main.c -o ecu
CMD ["./ecu"]
