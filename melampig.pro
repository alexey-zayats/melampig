
TEMPLATE=subdirs
SUBDIRS=src

isEmpty(PROJECT_PATH) {
	error(Usage: qmake -r PROJECT_PATH=/path/to/melampig)
}
