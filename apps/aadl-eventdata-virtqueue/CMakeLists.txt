#
# Copyright 2018, Data61
# Commonwealth Scientific and Industrial Research Organisation (CSIRO)
# ABN 41 687 119 230.
#
# This software may be distributed and modified according to the terms of
# the BSD 2-Clause license. Note that NO WARRANTY is provided.
# See "LICENSE_BSD2.txt" for details.
#
# @TAG(DATA61_BSD)
#

cmake_minimum_required(VERSION 3.7.2)

project(aadl-eventdata-virtqueue C)

#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/components/Receiver)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/components/Sender)

set(CAmkESCPP ON CACHE BOOL "" FORCE)
includeGlobalComponents()
DeclareCAmkESComponent(Receiver 
	SOURCES components/Receiver/src/receiver.c
	        LIBS virtqueue vswitch)
	DeclareCAmkESComponent(Sender 
		SOURCES components/Sender/src/sender.c
	        LIBS virtqueue vswitch)


DeclareCAmkESRootserver(aadl-eventdata-virtqueue.camkes)
