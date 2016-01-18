Created By Andrew Socha

Extra command is "blah"

known bugs:
- clients may not realize their connection has been broken until they attempt to send another message
- I tried to make the serverquit command send messages to each client to close gracefully before the server closes, but they will instead
just lose connection as they don't receive their message before the server calls Deinit (it works if you breakpoint before Deinit though!).