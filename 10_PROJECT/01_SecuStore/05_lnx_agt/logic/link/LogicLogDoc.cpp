/*
 * Copyright (C) 2021-2026 ASHINi Corp. 
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; either 
 * version 3 of the License, or any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public 
 * License along with this program. If not, see <https://www.gnu.org/licenses/>.   
 *  
 * 
 */

//---------------------------------------------------------------------------


#include "stdafx.h"
#include "com_struct.h"
#include "LogicLogDoc.h"

//---------------------------------------------------------------------------

CLogicLogDoc*		t_LogicLogDoc = NULL;

//---------------------------------------------------------------------------

CLogicLogDoc::CLogicLogDoc()
{	
	m_strLogicName = "logic log doc";
}
//---------------------------------------------------------------------------

CLogicLogDoc::~CLogicLogDoc()
{
}
//---------------------------------------------------------------------------

void		CLogicLogDoc::AnalyzePkt_LogDoc(PPKT_DATA pkt_data)
{
	InitBaseMember(pkt_data);
	switch(m_nPktCode)
    {
		case G_CODE_COMMON_UPDATE:									break;
		default:
        {
        	WriteLogE("[%s] not define pkt code from console : [%d][%d]", m_strLogicName.c_str(), m_nPktType, m_nPktCode);
			break;
        }
    }
	InitBaseMember();
    return;
}
//---------------------------------------------------------------------------



