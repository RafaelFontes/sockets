#!/usr/bin/perl -w

use strict;
use warnings;

use File::Copy;

mkdir "include";
mkdir "include/server";

my @files = glob("./socket/*.h");

foreach (@files) {
  copy("$_","./include/server.");
}

@files = glob("./protocol/*.h");

foreach (@files) {
  copy("$_","./include/protocol.");
}

copy("./socket/libsocket.lib","r:/build/server/release/lib/libsocket.lib");
