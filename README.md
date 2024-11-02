# terminal-blackjack

***Play blackjack with the terminal.***

This is a simple blackjack simulator for the terminal coded in
just one C++ file.

## Features

- Playable blackjack CLI game
- Input checking
- **Real simulated 52 card deck that makes card counting possible**
- *Advanded blackjack features:* insurance, double down, ~~split~~

### Roadmap

- [ ] Split hand
- [ ] Save and load game

## Install

This will install the executable in `/usr/local/bin`.

Use the `INSTALL.sh` script:

```bash
# Make sure to run this in the root of the repo
sh scripts/INSTALL.sh
```

Or install manually:

```bash
sudo g++ src/main.cpp -o /usr/local/bin/blackjack
```

## Build

This will build and run the executable.

```bash
g++ src/main.cpp -o blackjack
./blackjack
```

## Screenshots

<img src="https://github.com/kubgus/terminal-blackjack/raw/main/screenshots/1.png" alt="Screenshot of terminal-blackjack" width="512">

## License

This project is licensed under the MIT License.

## Contributing

Please fork this repo and create a pull request.
