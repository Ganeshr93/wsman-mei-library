/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2010-2019 Intel Corporation
 */
/*++

@file: GetMESetupAuditRecordCommand.h

--*/

#ifndef __GET_ME_SETUP_AUDIT_RECORD_COMMAND_H__
#define __GET_ME_SETUP_AUDIT_RECORD_COMMAND_H__

#include "AMTHICommand.h"
#include "MEIparser.h"
#include "AmtAnsiString.h"

namespace Intel
{
	namespace MEI_Client
	{
		namespace AMTHI_Client
		{
			typedef enum _CERT_HASH_ALGORITHM
			{ // compatible with AMT 6.0 and up
				CERT_HASH_ALGORITHM_MD5 = 0,  // 16 bytes
				CERT_HASH_ALGORITHM_SHA1,     // 20 bytes
				CERT_HASH_ALGORITHM_SHA256,   // 32 bytes
				CERT_HASH_ALGORITHM_SHA384,   // 48 bytes
			} CERT_HASH_ALGORITHM;

			typedef struct _CFG_TIMEDATE
			{
				uint16_t	Year;
				uint16_t	Month;
				uint16_t	DayOfWeek;
				uint16_t	Day;
				uint16_t	Hour;
				uint16_t	Minute;
				uint16_t	Second;
			} TIME_DATE;

			struct GetMESetupAuditRecord_RESPONSE
			{
				uint8_t					ProvisioningTLSMode;
				bool					SecureDNS;
				bool					HostInitiated;
				CERT_HASH_ALGORITHM		SelectedHashType;	
				struct 
				{
					uint8_t				SelectedHashData[64];
				} SelectedHashData;
				struct 
				{
					uint8_t				CaCertificateSerials[48];
				} CaCertificateSerials;
				bool					AdditionalCaSerialNums;
				bool					IsOemDefault;
				bool					IsTimeValid;
				std::string				ProvServerIP; 
				TIME_DATE				TlsStartTime;
				std::string				ProvServerFQDN;

				void parse (std::vector<uint8_t>::const_iterator& itr, const std::vector<uint8_t>::const_iterator end) 
				{
					Intel::MEI_Client::parseData( ProvisioningTLSMode,    itr, end);
					Intel::MEI_Client::parseData( SecureDNS,              itr, end);
					Intel::MEI_Client::parseData( HostInitiated,          itr, end);
					Intel::MEI_Client::parseData( SelectedHashType,       itr, end);
					Intel::MEI_Client::parseData( SelectedHashData,       itr, end);
					Intel::MEI_Client::parseData( CaCertificateSerials,   itr, end);
					Intel::MEI_Client::parseData( AdditionalCaSerialNums, itr, end);
					Intel::MEI_Client::parseData( IsOemDefault,           itr, end);
					Intel::MEI_Client::parseData( IsTimeValid,            itr, end);
					CFG_IP_ADDR ipAddr;
					Intel::MEI_Client::parseData( ipAddr,           itr, end);
					ProvServerIP = ipAddr.toString();
					Intel::MEI_Client::parseData( TlsStartTime,           itr, end);
					ProvServerFQDN = AmtAnsiString(itr, end).getString();
				}
			};

			class GetMESetupAuditRecordRequest;
			class GetMESetupAuditRecordCommand : public AMTHICommand
			{
			public:

				GetMESetupAuditRecordCommand();
				virtual ~GetMESetupAuditRecordCommand() {}

				GetMESetupAuditRecord_RESPONSE getResponse();

			private:
				virtual void parseResponse(const std::vector<uint8_t>& buffer);

				std::shared_ptr<AMTHICommandResponse<GetMESetupAuditRecord_RESPONSE>> m_response;
				
				static const uint32_t RESPONSE_COMMAND_NUMBER = 0x04800050;
			};

			class GetMESetupAuditRecordRequest : public AMTHICommandRequest
			{
			public:
				GetMESetupAuditRecordRequest() {}
				virtual ~GetMESetupAuditRecordRequest() {}

			private:
				static const uint32_t REQUEST_COMMAND_NUMBER = 0x04000050;
				virtual unsigned int requestHeaderCommandNumber()
				{
					//this is the command number (taken from the AMTHI document)
					return REQUEST_COMMAND_NUMBER;
				}

				virtual uint32_t requestDataSize()
				{
					return 0;
				}
				virtual std::vector<uint8_t> SerializeData();
			};
		} // namespace AMTHI_Client
	} // namespace MEI_Client
} // namespace Intel

#endif //__GET_ME_SETUP_AUDIT_RECORD_COMMAND_H__