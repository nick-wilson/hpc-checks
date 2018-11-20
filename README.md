# HPC Cluster Checks

This directory contains a series of tests which are run periodically to monitor the performance of the system over time and check change control.

This suite of tests is meant to be complementary to system monitoring checks performed by tools such as Zabbix. These tests focus on the functionality and performance of the service from an end-user perspective.

There are a mixture of functionality tests and performance tests.
Ideally every compiler, MPI library and application installed on the system would have it's own check but that is not always feasible.
The current suite covers a representative sample of software functionality tests, configuration checks and several critical performance metrics.

Each test is written so that there is a positive check that the test has passed as absence of an error code from an application does not mean it worked correctly.
Where applicable the performance of a benchmark or the time of execution is used as the pass/fail metric.

The jobs are run through the PBS job scheduler via cron scripts.
The standard tests are run daily, a few tests are run more frequently, some tests are run weekly and some are run on demand.

The directory structure is as follows:
```
tests		individual test scripts
data		input data for tests
etc/crontab	cron schedule
run		output data from tests
results.csv	summary of test results
sanity-check	check that data in results.csv matches metrics in output files
```
