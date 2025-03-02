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


#ifndef MEM_CHK_EXIST_INTERVAL_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467
#define MEM_CHK_EXIST_INTERVAL_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467

typedef struct _mem_chk_exist_interval
{
	_mem_chk_exist_interval()
	{
		nID					= 0;
		strChkName			= "";

		nStatus				= 0;
	}

	UINT32			nID;	
	String			strChkName;

	UINT32			nStatus;

}MEM_CHK_EXIST_INTERVAL, *PMEM_CHK_EXIST_INTERVAL;

typedef list<MEM_CHK_EXIST_INTERVAL>				TListMemChkExistInterval;
typedef TListMemChkExistInterval::iterator			TListMemChkExistIntervalItor;

typedef map<UINT32, MEM_CHK_EXIST_INTERVAL>			TMapMemChkExistInterval;
typedef TMapMemChkExistInterval::iterator			TMapMemChkExistIntervalItor;

#endif //MEM_WIN_SESSION_H_4D39CDDB_CTL_E289_4d56_9F98_FE42776F4467



