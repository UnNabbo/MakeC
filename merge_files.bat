@echo off
pushd "makec/src/"
type  base.h array.c strings.h args.h project.h cfg_parser.h strings.c utils.c os.c args.c msvc.c project.c cfg_parser.c entry.c > ../../makec.h 2> nul
popd
