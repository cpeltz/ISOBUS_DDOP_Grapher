/*
Copyright (c) 2015 jacob.finger@agcocorp.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "node.h"

#include <iostream>

#define CONTROL_SOURCE	( 1 << 7 )
#define SETTABLE		( 1 << 6 )
#define DEFAULT_SET		( 1 << 5 )
#define TOTAL			( 1 << 4 )
#define ON_CHANGE		( 1 << 3 )
#define THRESHOLD		( 1 << 2 )
#define DISTANCE		( 1 << 1 )
#define TIME			( 1 << 0 )

void
Node::addDdi( DDI* ddi )
{
	m_Ddis.push_back( ddi );
}

void
Node::addRef( int ref )
{
	m_Refs.push_back( ref );
}

void
DDI::print( std::ostream& out )
{
	std::string tableCell = "\t\t<TD";
	if( m_IsDpt )
	{
		tableCell += " BGCOLOR=\"grey\"";
	}
	else
	{
		if( m_Value & SETTABLE )
		{
			tableCell += " BGCOLOR=\"#00B0F0\"";
		}
	}

	out << "\n\t<TR>\n" << tableCell << ">" << ( m_IsDpt ? "DPT" : "DPD" )  << "</TD>\n"
		<< tableCell << ">" << m_ObjId << "</TD>\n"
		<< tableCell << " COLSPAN=\"8\">" << m_Name << "</TD>\n" 
		<< tableCell << ">" << std::hex << m_DDI << std::dec << "</TD>\n"
		<< tableCell << " COLSPAN=\"2\">";

	if( m_IsDpt )
	{
		out << m_Value;
	}
	else
	{
		int first = 0;
		if( m_Value & SETTABLE )
		{
			out << "S";
		}
		if( m_Value & DEFAULT_SET )
		{
			out << "D";
		}
		if( m_Value & TOTAL )
		{
			unsigned char sigma[] = { 0xce, 0xa3 };
			out << sigma[0] << sigma[1];
		}
		if( m_Value & ON_CHANGE )
		{
			out << "C";
		}
		if( m_Value & THRESHOLD )
		{
			unsigned char plusMinus[] = { 0xc2, 0xb1 };
			out << plusMinus[0] << plusMinus[1];
		}
		if( m_Value & DISTANCE )
		{
			unsigned char distance[] = { 0xE2, 0x86, 0x92 };
			out << distance[0] << distance[1] << distance[2];
		}
		if( m_Value & TIME )
		{
			out << "T";
		}
	}
	out << "</TD>\n\t</TR>";
}

void
Node::printDdis( std::ostream& out )
{
	for( unsigned i = 0; i < m_Ddis.size(); ++i )
	{
		m_Ddis[i]->print( out );
	}
}

void
Node::print( std::ostream& out )
{
	out << "\t" << m_ObjId 
		<< " [fontsize=\"10.00\", "
		<< "label=<\n<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" COLOR=\"black\" BGCOLOR=\"white\">\n"
		<< "\t<TR>\n\t\t<TD COLSPAN=\"2\">" << m_ObjId << "</TD>\n"
		<< "\t\t<TD COLSPAN=\"9\">" << m_Name << "</TD>\n"
		<< "\t\t<TD COLSPAN=\"2\">" << m_Element << "</TD>\t</TR>";
	out << "\n\t<TR>\n\t\t<TD>" << "Type" << "</TD>\n"
		<< "\t\t<TD>" << "ObjID" << "</TD>\n"
		<< "\t\t<TD COLSPAN=\"8\">" << "Label" << "</TD>\n" 
		<< "\t\t<TD>" << "DDI" << "</TD>\n"
		<< "\t\t<TD COLSPAN=\"2\">" << "Properties" << "</TD>\n\t</TR>";
	printDdis( out );
	out << "\n</TABLE>>, shape=" << getShape()
		<< "]"	<< std::endl;
}

const std::string&
Node::getShape()
{
	static std::string shapes[] = 
	{
		"ellipse", //1
		"box", //2
		"hexagon", //3
		"house", //4
		"ocatagon", //5
		"pentagon", //6
		"egg" //7
	};

	return shapes[m_Type-1];
}
