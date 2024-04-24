@echo off

set _current_dir=%CD%

powershell -Command "Start-Process cmd -ArgumentList '/C cd /d %_current_dir% & vcperf /start Profiling & call build & vcperf /stop Profiling /timetrace output_performance.json' -Verb RunAs"

echo execution terminated
