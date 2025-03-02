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
#include "LogicMgrPoInPtnWL.h"

//---------------------------------------------------------------------------

CLogicMgrPoInPtnWL*		t_LogicMgrPoInPtnWL = NULL;

//---------------------------------------------------------------------------

CLogicMgrPoInPtnWL::CLogicMgrPoInPtnWL()
{
	t_ManagePoInPtnWL		= new CManagePoInPtnWL();
	t_ManagePoInPtnWLPkg	= new CManagePoInPtnWLPkg();
	t_ManagePoInPtnWLUnit	= new CManagePoInPtnWLUnit();

	t_LogicMgrPtnGWO		= new CLogicMgrPtnGWO();


	t_ManagePoInPtnWL->LoadDBMS();
	t_ManagePoInPtnWLPkg->LoadDBMS();
	t_ManagePoInPtnWLUnit->LoadDBMS();

	t_ManagePoInPtnWL->InitHash();

	m_strLogicName		= "mgr agt po in ptn wl";
	
	m_nPolicyType		= SS_POLICY_TYPE_IN_PTN_WL;
	m_nSSPoSeqIdx		= SS_POLICY_INDEX_IN_PTN_WL;
	m_nAgtPktEditCode	= G_CODE_COMMON_EDIT;

	m_nEvtObjType		= EVENT_OBJECT_TYPE_POLICY;
	m_nEvtObjCode		= EVENT_OBJECT_CODE_POLICY_IN_PTN_WL;
}
//---------------------------------------------------------------------------

CLogicMgrPoInPtnWL::~CLogicMgrPoInPtnWL()
{
	SAFE_DELETE(t_LogicMgrPtnGWO);

	SAFE_DELETE(t_ManagePoInPtnWLUnit);
	SAFE_DELETE(t_ManagePoInPtnWLPkg);
	SAFE_DELETE(t_ManagePoInPtnWL);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
INT32		CLogicMgrPoInPtnWL::AnalyzePkt_FromMgr_Ext()
{
	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnWL::AnalyzePkt_FromMgr_Edit_Ext()
{
	PDB_PO_IN_PTN_WL pdata = NULL;
	DB_PO_IN_PTN_WL data;

	m_tDPH = &(data.tDPH);

	TListDBPoInPtnWLPkg		tPkgList;
	TListDBPoInPtnWLUnit	tUnitList;

	if( t_ManagePoInPtnWL->GetPkt(RecvToken, data))				return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	if(!RecvToken.TokenDel_32(m_nRecvNum))						return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
	while(m_nRecvNum--)
	{
		DB_PO_IN_PTN_WL_PKG	data_pkg;		
		DB_PO_IN_PTN_WL_UNIT data_unit;

		if( t_ManagePoInPtnWLPkg->GetPkt(RecvToken, data_pkg))		return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);
		if( t_ManagePoInPtnWLUnit->GetPkt(RecvToken, data_unit))	return SetHdrAndRtn(AZPKT_CB_RTN_PKT_INVALID);

		tPkgList.push_back(data_pkg);
		tUnitList.push_back(data_unit);

		data.tDPH._add_id(data_pkg.tDPH.nID);
	}

	pdata = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(pdata)
	{
		t_ManagePoInPtnWLPkg->ClearItemByPolicyID(pdata->tDPH.nID);		
		t_ManagePoInPtnWL->DelPoInPtnWL(pdata->tDPH.nID);
	}

	{
		{
			TListDBPoInPtnWLUnitItor begin, end;
			begin = tUnitList.begin();	end = tUnitList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInPtnWLUnit->ApplyPoInPtnWLUnit(*begin))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] apply policy unit information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}				
			}
		}

		{
			TListDBPoInPtnWLPkgItor begin, end;
			begin = tPkgList.begin();	end = tPkgList.end();
			for(begin; begin != end; begin++)
			{
				if(t_ManagePoInPtnWLPkg->FindItem(begin->tDPH.nID))
				{
					SetDLEH_EC(g_nErrRtn);
					WriteLogE("[%s] add policy pkg information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
					continue;
				}

				t_ManagePoInPtnWLPkg->AddPoInPtnWLPkg(*begin);
			}
		}

		if(SetER(t_ManagePoInPtnWL->ApplyPoInPtnWL(data)))
		{
			SetDLEH_EC(g_nErrRtn);
			WriteLogE("[%s] apply policy information : [%d]", m_strLogicName.c_str(), g_nErrRtn);
			return SetHdrAndRtn(AZPKT_CB_RTN_DBMS_FAIL);
		}
	}

	return SetHdrAndRtn(AZPKT_CB_RTN_SUCCESS);
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnWL::ApplyPolicy()
{
	PDB_PO_IN_PTN_WL pdata = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoInPtnWL->IsValidPtnFile(pdata) == 0 && t_EnvInfoOp->m_nApplyPolicyStatus)
			{
				AddDpDownInfo();
			}
			else
			{
				if(t_LogicMgrPtnGWO->ReloadPtnGWO())
				{
					WriteLogE("[%s] can not reload gwo ptn file  : [%s]", m_strLogicName.c_str(), pdata->tDPFI.strSaveName.c_str());
					return 0;
				}
				if(t_ThreadPoInAccFile != NULL)
					t_ThreadPoInAccFile->SendPolicy(AS_SEND_WHITE_FILE | AS_SEND_BLACK_FILE);
				t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
			}
		}
		else
		{
			t_ThreadTimer->t_TimerUtil.EnableTimer(TIMER_ID_POLICY_APPLY_EPS);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnWL::CheckRunEnv()
{
	PDB_PO_IN_PTN_WL pdata = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF && t_ManagePoInPtnWL->IsValidPtnFile(pdata) == 0)
		{
			AddDpDownInfo();
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnWL::LoadPtn()
{
	PDB_PO_IN_PTN_WL pdata = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)	
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return 0;
	}

	{
		if(pdata->tDPH.nUsedMode != STATUS_USED_MODE_OFF)
		{
			if(t_ManagePoInPtnWL->IsValidPtnFile(pdata) == 0)
			{
				if(t_EnvInfoOp->m_nApplyPolicyStatus)
				{
					AddDpDownInfo();
				}
			}
			else
			{
				if(t_LogicMgrPtnGWO->ReloadPtnGWO())
				{
					WriteLogE("[%s] can not reload gwo ptn file  : [%s]", m_strLogicName.c_str(), pdata->tDPFI.strSaveName.c_str());
					return 0;
				}
/*
				if(t_ThreadPoInAccFile != NULL)
					t_ThreadPoInAccFile->SendPolicy(AS_SEND_WHITE_FILE | AS_SEND_BLACK_FILE);
*/
			}
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnWL::OnTimer_Logic()
{
	return 0;
}
//---------------------------------------------------------------------------

INT32		CLogicMgrPoInPtnWL::AddDpDownInfo()
{
	char acPtnFile[MAX_PATH] = {0,};
	PDB_PO_SVR_INFO_UDT pdata_us = (PDB_PO_SVR_INFO_UDT)t_DeployPolicyUtil->GetCurPoPtr(SS_POLICY_TYPE_SVR_INFO_UDT);
	if(!pdata_us)
	{
		WriteLogE("[%s] not find policy svr info udt", m_strLogicName.c_str());
		return -1;
	}
	// 	else if(pdata_us->tDPH.nUsedMode != STATUS_USED_MODE_ON)
	// 	{
	// 		WriteLogE("[%s] not used udt chk svr info list", m_strLogicName.c_str());
	// 		return -2;
	// 	}
	else if(pdata_us->strDataSvrInfoList.empty())
	{
		WriteLogE("[%s] not exists udt chk svr info list", m_strLogicName.c_str());
		return -2;
	}

	PDB_PO_IN_PTN_WL pdata = (PDB_PO_IN_PTN_WL)t_DeployPolicyUtil->GetCurPoPtr(m_nPolicyType);
	if(!pdata)
	{
		UINT32 nPolID = t_DeployPolicyUtil->GetCurPoID(m_nPolicyType);
		if(nPolID != 0)
			WriteLogE("[%s] not find current policy (%d)", m_strLogicName.c_str(), nPolID);
		return -3;
	}

	if(pdata->tDPFI.strSaveName.empty() || pdata->tDPFI.strFileHash.empty())
	{
//		WriteLogE("[%s] invalid ptn file info [%s][%s]", m_strLogicName.c_str(), pdata->tDPFI.strSaveName.c_str(), pdata->tDPFI.strFileHash.c_str());
		return -4;
	}

	{
		ASI_FDL_INFO tAFI;

		tAFI.nID = t_EnvInfoOp->GetGlobalID();
		tAFI.nItemType = ASIFDL_DL_FILE_TYPE_WPTN_GWO;
		tAFI.nItemID = 0;	
		tAFI.nPolicyID = pdata->tDPH.nID;
		tAFI.nDLSvrUsedFlag	= (ASIFDL_DL_SVR_TYPE_LOCAL | ASIFDL_DL_SVR_TYPE_PTOP | ASIFDL_DL_SVR_TYPE_SITE);

		strncpy(tAFI.szDLPath, STR_DEPLOY_FILE_HOME_WPTN, MAX_PATH-1);

		{	
			strncpy(tAFI.szFileName, pdata->tDPFI.strSaveName.c_str(), MAX_PATH-1);
			strncpy(tAFI.szFileHash, pdata->tDPFI.strFileHash.c_str(), MAX_PATH-1);
		}	

		snprintf(acPtnFile, MAX_PATH-1, "%s/%s/%s", t_EnvInfo->m_strRootPath.c_str(), STR_WENG_PTN_FILE, tAFI.szFileName);

		if(is_file(acPtnFile) == REG_FILE)
		{
			if(t_ManageFileDown->IsExistDnInfo(tAFI.nItemType, tAFI.nPolicyID, tAFI.nItemID))
			{
				WriteLogN("already exists in ptn wl download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, acPtnFile);
				return 0;
			}
		}
		WriteLogN("start in ptn wl download : [%d][%d][%s]:[%s]", tAFI.nID, tAFI.nItemID, tAFI.szFileName, pdata_us->strDataSvrInfoList.c_str());
		SetDLSvrInfo(ASIFDL_DL_SVR_TYPE_SITE, pdata_us->strDataSvrInfoList.c_str());
		AddDLInfo(&tAFI);

		t_ManageFileDown->AddItem(tAFI);
	}
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
