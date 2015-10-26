/* 
 * File:   PhysicalProcess.cc
 * Author: sha
 * 
 * Created on May 4, 2014, 9:38 AM
 */

#include "PhysicalProcess.h"

PhysicalProcess::PhysicalProcess(const char* process_name, int code, const char* graph_title):
	chi2(0)
{
    name = process_name;
    dataCode = code; 
    title = graph_title;
    numberOfpoints = 0; 
}
