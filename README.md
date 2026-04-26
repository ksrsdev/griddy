# griddy
gridiron simulator

Insipred by the all 22 next gen stats "dots" representation of gridiron plays. The idea is a mix between tecmo bowl and OOTP. Currently in a very early stage and I'm learning as I go. The goal is to write the entire program in pure C with no crashes or "at fault" memory leaks.

# License
This project is released under the 0BSD license. You can use, copy, modify, and distribute the code and assets for any purpose, commercial or private, without any restrictions or attribution requirements.

Essentially: Do whatever you want. See the LICENSE file for the full legal text.

# Software 

C23
SDL3
SDL_shadercross (Planning to remove this as a compile time depend...eventually to keep binary size small and simplify install / build steps)

I want to keep this as simple as I can while still making something that is accessible to the broadest group of potential end users I am only a linux user so that's my dev environment but as I get closer to release time I will test these things on windows and mac. I can't promise to maintain build instructions for other platforms right now (in fact I don't even have written build instructions for linux but that's on the short list)

I chose to use the most recent versions of C and SDL because this is primarily a learning exercise for me at this point and they both have some quite nice QoL features in the new releases. The way I handle the SDF font specifically wouldn't work in earlier versions of SDL (there's probably /some/ solution) and nullptr and constexpr are enough for me to enjoy C23 more than earlier versions.

I currently depend on shadercross to compile the SDF shader at run time. This may change in the future. 

You will need a GPU backend to execute the program. vulkan-radeon is the package I use on arch with a 6600

# Build Instructions

To be written :D

# Release Goals

Alpha: A playable game loop

Beta: A full simulation core, animated plays, teams with "personality", Color Bowl mode

1.0 Release:
    Full multi year dynasy mode - fixed "color" league is ok

Additional features I'm targetting:

+ dynamic / custom league types
+ team culture
+ graphics and audio
+ text to speech announcers
+ practice, coaches, trainers etc
+ play designer
+ weather







