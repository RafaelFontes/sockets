TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = socket protocol test

test.depends = socket protocol
protocol.depends = socket

DISTFILES+="*.pl"
