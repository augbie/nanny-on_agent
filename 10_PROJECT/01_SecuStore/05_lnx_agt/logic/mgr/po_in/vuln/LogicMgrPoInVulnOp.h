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

#ifndef LogicMgrPoInVulnOpH
#define LogicMgrPoInVulnOpH
//---------------------------------------------------------------------------

class CLogicMgrPoInVulnOp	: public CLogicBase
{
private:
	INT32		AnalyzePkt_FromMgr_Ext();
	INT32		AnalyzePkt_FromMgr_Edit_Ext();

public:
	INT32		ApplyPolicy();
	INT32		CheckRunEnv();

	INT32		PreScanVuln();
	INT32		SimpleScanVuln(UINT32& nDangerVulnID);

private:
	INT32		AddDpDownInfo();

public:
	CLogicMgrPoInVulnOp();
    ~CLogicMgrPoInVulnOp();
};

extern CLogicMgrPoInVulnOp*		t_LogicMgrPoInVulnOp;

#endif
