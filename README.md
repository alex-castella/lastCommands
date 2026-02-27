In order to use this program you first need to modify the `PROMPT_COMMAND` variable in $HOME/.bashrc. 

Add this line to the end of the $HOME/.bashrc:
```bash
export PROMPT_COMMAND="history -a; ${PROMPT_COMMAND}"
