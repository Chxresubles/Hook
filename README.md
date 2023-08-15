# Hook
Hook is a music streaming REST web server made in C using mongoose

# Disclaimer
This project is still in development. Some functionalities are missing or not working properly.

# Prerequisites
To compile and run this program, you will need to install a C compiler.

# Compile and run

To run this program, follow these steps:
1. Clone this repo
```bash
$ git clone https://github.com/Chxresubles/Hook.git
$ cd Hook
```
2. Configure CMake
```bash
$ cmake -B build
```
3. Compile the Hook code
```bash
$ make -C build
```
4. Install the Hook binaries
```bash
$ make -C build install DESTDIR="<destination/dir>"
```
3. Run the compiled executable
```bash
$ cd <destination/dir>
$ ./bin/Hook
```

The server will listen on http://127.0.0.1:80. It will list the files present in the `songs` directory.

# Server API

The [server API](swagger_api.json) is described using a [Swagger API](https://swagger.io/).

# Made with
This project was made using:
* [mongoose](https://github.com/cesanta/mongoose)