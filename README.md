# Hook
Hook is a music streaming REST web server made in C++ using mongoose

# Disclaimer
This project is still in development. Some functionalities are missing or not working properly.

# Prerequisites
To compile and run this program, you will need to install the C/C++ compiler g++.

# Compile and run

To run this program, follow these steps:
1. Clone this repo
```bash
$ git clone https://github.com/Chxresubles/Hook.git
$ cd Hook
```
2. Compile the Hook code
```bash
$ make Hook
```
3. Run the compiled executable
```bash
$ build/Hook.exe
```

The server will listen on http://127.0.0.1:80. It will list the files present in the `build` directory.

# Server API

The [server API](swagger_api.json) is described using a [Swagger API](https://swagger.io/).

# Made with
This project was made using:
* [mongoose](https://github.com/cesanta/mongoose)