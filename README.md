# StudioPatcher
This is an alternative way to force Roblox Studio to run in internal mode, since hex editing didn't work for me (I could not obtain the byte sequences necessary)
This could *probably* be improved in some way? I'm not too certain, but it's in a functional state.

# How to update this
You will need IDA (Interactive Disassembler) or similar software which allows you to find references to strings.
Search up "GameScript" in the applications' strings and go to the first cross reference.

Press Tab to decompile the function in question.
You should see something similar to this:
![image](https://github.com/the-wawa/StudioPatcher/assets/83979811/f4d53ebb-89b6-4808-8389-708687afbfed)

You are interested in the if-statement that looks like this:
```cpp
if ( sub_XXXXXXXXX() )
{
```

Go to this function.
![image](https://github.com/the-wawa/StudioPatcher/assets/83979811/d7814fed-bf55-475d-9832-05c84448d653)

You should see something similar to this.

Finally, these two variables contain our memory addresses.
Modify Addresses.h to include them and re-build the program to (hopefully) get a working patcher.
