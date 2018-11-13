# Audio Master
A 3rd year project for Software Engineering BSc course.

# Project Overview
This project is a High Quality Digital Audio Workstation (DAW) that allows users to record and playback music, add layered effects, and create tablature/stave notation based on the recordings.

# Goals
Interactive and intuitive user interface
Import/export data
Sound recording/playback
Waveform display (raw data view)
Stave display (estimation of notation from raw data)
Tablature display (estimation of notation from raw data in a different view)
Sound effect layering

# GUI Design
I am using wxWidgets to develop the user interface, this is because it is a cross-platform library with limited restrictions, open source, and a slightly adapted L-GPL licence (meaning it is legally safe to use for my project so long as I reference the use of the library).

I will have: toolbars for recording and playback controls, and input and output controls; menus for controlling project data, exporting/importing, and effects; and scrollable panels for the waveform display (direct view of recorded/imported data) and the stave/tab display (display of the closest estimation of the notation of the recorded/imported data).

# Sound Manager
I will be using OpenAL to develop the sound manager as it is cross-platform, widely available and accessible.
The sound manager will need to be able to manage Input/Output devices, raw data access for displays, effect layering (requires the effects extension in OpenAL).