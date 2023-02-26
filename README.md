# Overview

A simplified analogue of the UNIX utility 'ls'.

# Build

In the source code directory run:

```cmake -S .```

```cmake --build .```

# Supported options

* -l: in addition to the name of each file, the file type, file permissions, number of links to the file, owner name, group name, file size in bytes, and timestamp (when the file was last modified) are displayed. For files with a time greater than 6 months ago or greater than 1 hour into the future, the timestamp contains the year instead of the time of day.

* -h: prefix each file size with a size letter, such as M for binary megabytes (`mebibyte').

* -r: sort the contents of a directory in reverse order.

--help: display this help and exit.

# Usages example

Simply output: ```ls```

Long format: ```ls -l /dir```

Long format output for two directories: ```ls /dir /dir2 -l```

Long format output and human-readable format for current dir(./): ```ls -l -h```

Long format output for directories and file, descending order and human-readable: ```ls -lh /file1.bin /dir ./ -r```

# LICENSE

This software is distributed under [MIT](https://opensource.org/licenses/MIT) license.
