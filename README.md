# ceptool
A checker for event sequence properties on a stream of events

This is a CEP (Complex Event Processing) tool that can check and report
violation of certain event sequence properties in a stream of events.

The properties can be specified in one of the two formats:

    1. DEP format : This is a higher level format in which event dependencies
       can be specified using some builtin operators.

    2. CEP format : This is a lower level format in which an event interval and
       a boolean expression in terms of aggregations over events is specified.
       The boolean expression is checked at the end of the interval and if it
       is found to be false, it is logged.

    The DEP spec is converted (or expanded) to CEP form. But one may also write
    CEP spec directly by hand. The run time system understands only the CEP
    spec.

    Both DEP and CEP specs are expressed in Prolog syntax as it makes it easy
    to express them. A detailed knowledge of Prolog language is not a
    requirement to write and understand these specs.

    See examples/depdat for sample DEP format and its explanation.

    A later section describes its conversion to the CEP format. The CEP format looks like:

        cep( Interval, Condition ).
        ...

        where Interval is a tuple of ( StartEvent, EndEvent )
        Start and End events are integers that identify the event types.  A
        special symbol 'START' represents a symbolic event which is deemed as
        happening strictly before any other event occurs.

        Conditional expression supports following operators:

        #E : Number of occurrences of event E in the interval
        ^  : Logical Xor
        == : Equality

## System requirements

    1. xsbcppif ( https://github.com/mayureshw/xsbcppif )
    Please check the system requirements of this component.

## Installation

    1. Choose your installation directory. E.g. $HOME/programs. Make sure that
       it is created and is writable.

    2. Clone the required components into your installation directory:

        git clone https://github.com/mayureshw/xsbcppif
        git clone https://github.com/mayureshw/ceptool

    3. Follow the installation setup instructions mentioned in
       xsbcppif/Makefile.xsbcppif.

       As a checklist of a few things (not necessarily exhaustive) check if you
       have done this:

            - XSB Prolog is installed and you can type xsb to run it
            - Environment variables in above instuctions are set and echoing
              them shows their value.
            - xsbrc.P is copied to $HOME/.xsb as mentioned in above instuctions

    4. Set environment variable CEPTOOLDIR to where you have cloned this code.
       ( E.g. $HOME/programs/ceptool ) 

    5. Add $CEPTOOLDIR to the XSBLIBPATH environment variable (which is a comma
       separated list of directories for xsb modules.)

    6. Optionally, add $CEPTOOLDIR to the PATH variable. Alternatively you will
       need to invoke the tools in this directory using fully qualified path.

    7. Optionally, in $CEPTOOLDIR run "make" to build the xwam files. (If you
       skip this step, the xwam files get built automatically during usage.
       Just that you will need write access to $CEPTOOLDIR at usage time.)

## Converting DEP format to CEP

    Shell script $CEPTOOLDIR/dep2cep converts a DEP formatted file to CEP
    format.

    E.g.
        $CEPTOOLDIR/dep2cep $CEPTOOLDIR/examples/depdat ./cepdat


## Checking event properties specified in a CEP file on a stream of events

    See examples/example.cpp to see how the CEP spec is checked on a sample
    event stream. Use examples/Makefile to build the example and see
    compilation settings.

    Before running example.out make sure that cepdat produced in above step is
    in your $XSBLIBPATH (or in your current directory).

    After running example.out a report will be produced in file cep.log

## Wish list

    As of now, the goal is to establish the framework and the flow. A very
    small number of operators are supported. The operators will grow over time.

    May consider adding event instance id which can help report the exact part
    of the event stream where the property was violated.

    May support naming the intervals, so that abstract events can be formed
    hierarchically.

    May add Action spec to elaborate the reporting.
