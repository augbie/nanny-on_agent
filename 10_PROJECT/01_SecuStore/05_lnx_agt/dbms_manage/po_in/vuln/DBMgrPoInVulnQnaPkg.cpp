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
#include "DBMgrPoInVulnQnaPkg.h"

//---------------------------------------------------------------------------

CDBMgrPoInVulnQnaPkg*		t_DBMgrPoInVulnQnaPkg = NULL;

//---------------------------------------------------------------------------

CDBMgrPoInVulnQnaPkg::CDBMgrPoInVulnQnaPkg()
{
	m_strDBTName = "po_in_vuln_qna_pkg";
}
//---------------------------------------------------------------------------

CDBMgrPoInVulnQnaPkg::~CDBMgrPoInVulnQnaPkg()
{
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnQnaPkg::SetInitalize()
{
    {
    	
	}
	//WriteLogN("current file dbms : [po_in_vuln_qna_pkg][%d]", m_nDBSVer);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnQnaPkg::LoadDB(TListDBPoInVulnQnaPkg& tDBPoInVulnQnaPkgList)
{
    UINT32 nReadCnt = 0;
    DB_PO_IN_VULN_QNA_PKG data;

    INT32 nIndex = 0;

    m_strQuery = SPrintf(DBMS_POLICY_QUERY_PKG_HDR_SELECT
						" FROM po_in_vuln_qna_pkg WHERE used_flag=1;");
    if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_SELECT_FAIL;

    do
    {
		nIndex = 0;
		DB_PO_HEADER& tDPH = data.tDPH;

		tDPH						= GetDBField_PoPkgHDR(nIndex);
		
        tDBPoInVulnQnaPkgList.push_back(data);
        if(m_nLoadMaxID < UINT32(tDPH.nID))	m_nLoadMaxID = tDPH.nID;
        nReadCnt++;
    }while(Next());
	m_nLoadNumber = tDBPoInVulnQnaPkgList.size();
	WriteLogN("load database : [%s][%u]", m_strDBTName.c_str(), m_nLoadNumber);
    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnQnaPkg::InsertPoInVulnQnaPkg(DB_PO_IN_VULN_QNA_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("INSERT INTO po_in_vuln_qna_pkg("
						DBMS_POLICY_QUERY_PKG_HDR_INSERT_NAME
						") VALUES (%s"
						");",
                        GetPoPkgHDRQuery_InsertValue(tDPH).c_str());

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_INSERT_FAIL;

	if(tDPH.nID == 0)
		tDPH.nID      = GetLastID();

    return 0;
}
//---------------------------------------------------------------------------

INT32			CDBMgrPoInVulnQnaPkg::UpdatePoInVulnQnaPkg(DB_PO_IN_VULN_QNA_PKG& data)
{
	DB_PO_HEADER& tDPH = data.tDPH;

	m_strQuery = SPrintf("UPDATE po_in_vuln_qna_pkg SET "
						"%s"
						" WHERE id=%u;",
						GetPoPkgHDRQuery_Update(tDPH).c_str(), 
						tDPH.nID);

	if(DBOP_Check(ExecuteQuery(m_strQuery)))		return ERR_DBMS_UPDATE_FAIL;

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

INT32	CDBMgrPoInVulnQnaPkg::LoadExecute(PVOID lpTempletList)
{
	TListDBPoInVulnQnaPkg* ptDBList = (TListDBPoInVulnQnaPkg*)lpTempletList;

    return LoadDB(*ptDBList);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnQnaPkg::InsertExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_QNA_PKG pd_t = (PDB_PO_IN_VULN_QNA_PKG)lpTemplet;

    return InsertPoInVulnQnaPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnQnaPkg::UpdateExecute(PVOID lpTemplet)
{
	PDB_PO_IN_VULN_QNA_PKG pd_t = (PDB_PO_IN_VULN_QNA_PKG)lpTemplet;

    return UpdatePoInVulnQnaPkg(*pd_t);
}
//---------------------------------------------------------------------------

INT32	CDBMgrPoInVulnQnaPkg::DeleteExecute(UINT32 nID)
{
	return CDBMgrBase::DeleteExecute(nID);
}
//---------------------------------------------------------------------------






