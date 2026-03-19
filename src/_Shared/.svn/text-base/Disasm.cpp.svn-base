#include "stdafx.h"
/*
     8888888b.                  888     888 d8b                        
     888   Y88b                 888     888 Y8P                        
     888    888                 888     888                            
     888   d88P 888d888  .d88b. Y88b   d88P 888  .d88b.  888  888  888 
     8888888P"  888P"   d88""88b Y88b d88P  888 d8P  Y8b 888  888  888 
     888        888     888  888  Y88o88P   888 88888888 888  888  888 
     888        888     Y88..88P   Y888P    888 Y8b.     Y88b 888 d88P 
     888        888      "Y88P"     Y8P     888  "Y8888   "Y8888888P"  


                        Dissasembler Engine Core
                        ~~~~~~~~~~~~~~~~~~~~~~~~

 Written by Bengaly (R) 2003-2005.
 As a part of the Proview (a.k.a PVDasm).
 Permission is granted to make and distribute verbatim copies of this
 Program provided the copyright notice and this permission notice are
 Preserved on all copies.

 File: Disasm.cpp (main)

 Disassembler Core Version: 1.04b
*/  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Disasm.h"
#define BYTES_TO_DECODE 16

const char *Regs[3][9] = {
    {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"},
    {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"},
    {"eax","ecx","edx","ebx","esp","ebp","esi","edi"} 
};

const char *RegSize[4] = {"Qword","Dword","Word","Byte"};
const char *Segs[8]    = {"ES","CS","SS","DS","FS","GS","SEG?","SEG?"};

void Decode(DISASSEMBLY *Disasm,char *Opcode,DWORD *Index)
{
	/*
	  This function is the Main decoding rutine.
	  The function gets 3 params:
	  1. DISASSEMBLY struct pointer
	  2. Opcode pointer, point to the linear address to decode
	  3. Index pointer, this is the distance from the beginning<>end of the linear

	  The function First searches for Prefixes + Repeated prefixes,
	  This is the first step to do in any disasm engine.
	  Prefixes determine behavior of instruction and the way they
	  Are decoded.
	  Once prefies has been found, we changes params (such as default r/m size, Lock..)

	  The function than searched for the byte to be decoded, the actual
	  Menemonic referenced in CPU form (Opcode),
	  I have not used big table - time prob -, although it is highly recomended! (hopefully in future)
    */

	// intializations
    DWORD dwMem=0,dwOp=0;
    int  i=*Index,RegRepeat=0,LockRepeat=0,SegRepeat=0,RepRepeat=0,AddrRepeat=0; // Repeated Prefixes    
	int  RM=REG32,SEG=SEG_DS,ADDRM=REG32;       // default modes	
	int PrefixesSize=0,PrefixesRSize=0;         // PrefixesSize = all Prefixes(no rep), PrefixesRsize (with Rep Prefix)
    WORD wMem=0,wOp=0;    
    bool RegPrefix=0,LockPrefix=0,SegPrefix=0,RepPrefix=0,AddrPrefix=0;  // default size of Prefixes
    BYTE Bit_D=0, Bit_W=0; // bit d/w for R/M
	char menemonic[256]="";
    char RSize[10]="Dword";                     // default size of menemonic
    BYTE Op=(BYTE)Opcode[i];                    // current opcode
    


    //=======================================================
    //               Decoding Prefixes                     //
    //=======================================================

    // we first assume there is prefix !
	// if we skip this, our decoding might be currupted

	while( // check only RegPreifix/LockProfix/SegPrefixes/RepPrefix/AddrPerfix
		   (Op==0x66) || (Op==0x0F0)|| (Op==0x2E) || (Op==0x36) ||
		   (Op==0x3E) || (Op==0x26) || (Op==0x64) || (Op==0x65) || 
		   (Op==0xF2) || (Op==0xF3) || (Op==0x67)
		 ) 
	{
		switch(Op)
		{
			case 0x66: // reg prefix, change default size, dword->word
			{                   
				RM=REG16; // 66 prefix, change default size
				RegPrefix=1; 
				BYTE temp;
				sprintf(RSize,"%s",RegSize[2]); // change default size of menemonic to 'Word'
				//LockPrefix=0;
				strcat(Disasm->Opcode,"66:");
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				temp=(BYTE)Opcode[i+1];
				RegRepeat++;
				if(RegRepeat>1) 
				{
					strcpy(Disasm->Opcode,"66:");
					strcpy(Disasm->Remarks,"Prefix DataSize:");
					Disasm->OpcodeSize=1;
					Disasm->PrefixSize=0;
					(*Index)-=RegRepeat;
					return;
				}
			}
			break;

			case 0x67: // Addr prefix, change default Reg size, (EDI->DI) and more!
			{
				ADDRM=REG16; // 67 prefix, change default size, in this case: Memory Reg Size
				AddrPrefix=1; 
				BYTE temp;
				strcat(Disasm->Opcode,"67:");
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				temp=(BYTE)Opcode[i+1];
				AddrRepeat++;
				if(AddrRepeat>1) 
				{
					strcpy(Disasm->Opcode,"67:");
					strcpy(Disasm->Remarks,"Prefix AddrSize:");
					Disasm->OpcodeSize=1;
					Disasm->PrefixSize=0;
					(*Index)-=AddrRepeat;
					return;
				}
			}
			break;

			case 0x0F0: // LockPrefix, Add bus lock menemonic opcode in front of every menemonic
			{
				//BYTE temp;
				LockPrefix=1;
				//RegPrefix=0; 
				strcat(Disasm->Opcode,"F0:");
				strcpy(Disasm->Assembly,"lock ");
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				//temp=(BYTE)Opcode[i+1];
				LockRepeat++;
				if(LockRepeat>1) 
				{
					strcpy(Disasm->Assembly,"");
					strcpy(Disasm->Opcode,"F0:");
					strcpy(Disasm->Remarks,"Prefix LOCK:");
					Disasm->OpcodeSize=1;
					Disasm->PrefixSize=0;
					(*Index)-=LockRepeat;
					return;
				}
			}
			break;

			case 0xF2: case 0xF3: // RepPrefix (only string instruction!!)
			{
                BYTE NextOp=(BYTE)Opcode[i+1];              // Next followed opcode
                BYTE NextOp2=(BYTE)Opcode[i+2];
				RepPrefix=1;
				sprintf(menemonic,"%02X:",Op);
				strcat(Disasm->Opcode,menemonic);
				switch(Op)
				{
					case 0xF2:sprintf(menemonic,"repne ");break;
					case 0xF3:sprintf(menemonic,"repe ");break;
				}
				strcat(Disasm->Assembly,menemonic);
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				RepRepeat++;
				// REPE/REPNE Prefixes affect only string operations:
				// MOVS/LODS/SCAS/CMPS/STOS/CMPSS.CMPPS..etc (NewSet Instructions)
				if(!( 
                      (Op>=0xA4 && Op<=0xA7) || 
                      (Op>=0xAA && Op<=0xAF) || 
                      (NextOp==0x0F && NextOp2==0x2A) ||
                      (NextOp==0x0F && NextOp2==0x10) ||
                      (NextOp==0x0F && NextOp2==0x11) ||
                      (NextOp==0x0F && NextOp2==0x2C) ||
                      (NextOp==0x0F && NextOp2==0x2D) ||
                      (NextOp==0x0F && NextOp2==0x51) ||
                      (NextOp==0x0F && NextOp2==0x52) ||
                      (NextOp==0x0F && NextOp2==0x53) ||
                      (NextOp==0x0F && NextOp2==0x58) ||
                      (NextOp==0x0F && NextOp2==0x59) ||
                      (NextOp==0x0F && NextOp2==0x5C) ||
                      (NextOp==0x0F && NextOp2==0x5D) ||
                      (NextOp==0x0F && NextOp2==0x5E) ||
                      (NextOp==0x0F && NextOp2==0x5F) ||
                      (NextOp==0x0F && NextOp2==0xC2) 
                    )
                  )
				{
					strcpy(Disasm->Assembly,"");
					strcpy(Disasm->Remarks,"Prefix REP:");
					Disasm->OpcodeSize=1;
					Disasm->PrefixSize=0;
					(*Index)-=RepRepeat;
					return;
				}
					
			}
			break;

			case 0x2E: case 0x36: // Segment Prefixes
			case 0x3E: case 0x26: // Segment Prefixes
			case 0x64: case 0x65: // Segment Prefixes
			{
				BYTE temp;
				switch(Op)
				{
					// Change Default Segment
					case 0x2E: SEG = SEG_CS; break; // Segment CS
					case 0x36: SEG = SEG_SS; break; // Segment SS
					case 0x3E: SEG = SEG_DS; break; // Segment DS
					case 0x26: SEG = SEG_ES; break; // Segment ES
					case 0x64: SEG = SEG_FS; break; // Segment FS
					case 0x65: SEG = SEG_GS; break; // Segment GS
				}

				SegPrefix=1;
				sprintf(menemonic,"%02X:",Op);
				strcat(Disasm->Opcode,menemonic);
				i++;
				++(*Index);
				Op=(BYTE)Opcode[i];
				temp=(BYTE)Opcode[i-2];
				SegRepeat++;

				// Check if SegPrefix is repeating
				if(SegRepeat>1) 
				{                    
					BYTE opc=(BYTE)Opcode[i-1];

					if(  temp==0x2E || temp==0x36 ||
					     temp==0x3E || temp==0x26 ||
                         temp==0x64 || temp==0x65 ||
                         temp==0x66 || temp==0xF0 ||
                         temp==0x67                        
					  )
					{
						// Check if last byte was an seg prefix and show it
						if(temp==0x66 || temp==0xF0 || temp==0x67)
                        {
                           opc=(BYTE)Opcode[i-3];
                           SegRepeat++;
                        }
                        else
                           opc=(BYTE)Opcode[i-2];

						switch(opc)
						{
							// Change Segment, accurding to last segPrefix (if repeated)
							case 0x2E: SEG = SEG_CS; break; // Segment CS
							case 0x36: SEG = SEG_SS; break; // Segment SS
							case 0x3E: SEG = SEG_DS; break; // Segment DS
							case 0x26: SEG = SEG_ES; break; // Segment ES
							case 0x64: SEG = SEG_FS; break; // Segment FS
							case 0x65: SEG = SEG_GS; break; // Segment GS
						}
					

                        strcpy(Disasm->Assembly,"");
                        sprintf(menemonic,"%02X:",opc);
                        strcpy(Disasm->Opcode,menemonic);
                        sprintf(menemonic,"Prefix %s:",Segs[SEG]);
                        strcpy(Disasm->Remarks,menemonic);
                        Disasm->OpcodeSize=0;
                        Disasm->PrefixSize=1;
                        (*Index)-=SegRepeat;
                    }
					return;
				}
			}
			break;
			
		    default:
			{
				// reset prefixes/repeats to default
				LockRepeat=0;
				RegRepeat=0;
				SegRepeat=0;
				RegPrefix=0;
				LockPrefix=0;
				SegPrefix=0;
				strcpy(RSize,RegSize[1]); // Default size
			}
			break;
		}
	}

    
	//=============================================
	//        Main Decoding starts here!         //
	//=============================================

	// Calculate Prefixes Sizes
	PrefixesSize  = RegPrefix+LockPrefix+SegPrefix+AddrPrefix; // No RepPrefix
	PrefixesRSize = PrefixesSize+RepPrefix; // Special Case (Rep Prefix is being used -> String menemonics only)

    switch(Op) // Find & Decode Big Set Opcodes
    {
		case 0x00: case 0x01: case 0x02: case 0x03: // ADD  XX/XXX, XX/XXX
		case 0x08: case 0x09: case 0x0A: case 0x0B: // OR   XX/XXX, XX/XXX
		case 0x10: case 0x11: case 0x12: case 0x13: // ADC  XX/XXX, XX/XXX 
		case 0x18: case 0x19: case 0x1A: case 0x1B: // SBB  XX/XXX, XX/XXX 
		case 0x20: case 0x21: case 0x22: case 0x23: // AND  XX/XXX, XX/XXX 
		case 0x28: case 0x29: case 0x2A: case 0x2B: // SUB  XX/XXX, XX/XXX 
		case 0x30: case 0x31: case 0x32: case 0x33: // XOR  XX/XXX, XX/XXX 
		case 0x38: case 0x39: case 0x3A: case 0x3B: // CMP  XX/XXX, XX/XXX 
        case 0x88: case 0x89: case 0x8A: case 0x8B: // MOV  XX/XXX, XX/XXX 
        case 0x8C: case 0x8E:                       // MOV  XX/XXX, XX/XXX
		case 0x62: case 0x63:                       // BOUND / ARPL XX/XXX, XX/XXX
        case 0x69:                                  // IMUL RM,IIM32 (DWORD)
        case 0x6B:                                  // IMUL <reg>,<RM>
		case 0x80: case 0x81: case 0x82: case 0x83: // MIXED Instructions
		case 0x84: case 0x85:                       // TEST
		case 0x86: case 0x87:                       // XCHG
        case 0x8D:                                  // LEA 
        case 0x8F:                                  // POP
        case 0xC0: case 0xC1:                       // MIXED Instructions
        case 0xC4: case 0xC5:                       // LES / LDS REG,MEM
        case 0xC6: case 0xC7:                       // MOV [MEM],IIM8/16/32
        case 0xD0: case 0xD1: case 0xD2: case 0xD3: // MIXED Bitwise Instructions
        case 0xD8: case 0xD9: case 0xDA: case 0xDB: // FPU Instructions
        case 0xDC: case 0xDD: case 0xDE: case 0xDF: // FPU Instructions
        case 0xF6: case 0xF7: case 0xFE: case 0xFF: // MIX Instructions
        {
			if(((BYTE)Opcode[i+1] & 0xC0)==0xC0)   // Check Opcode Range
			{
				char mene[10]="";
				
				GetInstruction(Op,mene); // Get instruction from Opcode Byte
				Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
				Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
				
				// Check Special Cases for alone Opcodes
				switch(Op)
				{
                    case 0x63:{Bit_D=0;Bit_W=1;}               break;
                    case 0x62:{Bit_D=1;Bit_W=1;}               break;
                    case 0x86:{Bit_D=0;Bit_W=0;}               break;
                    case 0x87:{Bit_D=0;Bit_W=1;}               break;
                    case 0x80: case 0x82: { Bit_D=0;Bit_W=0; } break;
                    case 0x81: case 0x83: { Bit_D=0;Bit_W=1; } break;
                    case 0x8C:{ Bit_D=0;Bit_W=0;}              break;
                    case 0x8E:{ Bit_D=1;Bit_W=0;}              break;
                    case 0xC4: case 0xC5: { Bit_D=1;Bit_W=1; } break;
				}

				Mod_11_RM(Bit_D,Bit_W,&Opcode,&Disasm,mene,RegPrefix,Op,&Index); // Decode with bits
				Disasm->PrefixSize=PrefixesSize; // PrefixSize (if prefix present)				
				break;
			}
			
			// operand doesn't have byte(s) extension in addressing mode
			if((BYTE)Opcode[i+1]>=0x00 && (BYTE)Opcode[i+1]<=0xBF)
			{
				char mene[10]="";
				GetInstruction(Op,mene); // Get instruction from Opcode Byte
				Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
				Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
				Mod_RM_SIB(&Disasm,&Opcode,i,AddrPrefix,SEG,&Index,Bit_D,Bit_W,mene,Op,RegPrefix,SegPrefix,AddrPrefix);
				Disasm->PrefixSize=PrefixesSize;
				break;
			}	  				
		}
		break;

		case 0x04:case 0x0C:case 0x14: // INSTRUCTION AL,XX
		case 0x1C:case 0x24:case 0x2C: // INSTRUCTION AL,XX
        case 0x34:case 0x3C:case 0xA8: // INSTRUCTION AL,XX
		case 0xE4:
        {
			char mene[10]="";
			GetInstruction(Op,mene); // Get instruction for a specified Byte
            sprintf(menemonic,"%s al, %02X",mene,(BYTE)Opcode[i+1]);
            strcat(Disasm->Assembly,menemonic);
            strcpy(Disasm->Remarks,"");
            sprintf(menemonic,"%02X%02X",Op,(BYTE)*(Opcode+i+1));
            strcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

        case 0x05:case 0x0D:case 0x15: // INSTRUCTION EAX/AX,XXXXXXXX
        case 0x1D:case 0x25:case 0x2D: // INSTRUCTION EAX/AX,XXXXXXXX
		case 0x35:case 0x3D:case 0xA9: // INSTRUCTION EAX/AX,XXXXXXXX
        {
			char mene[10]="";
			GetInstruction(Op,mene); // Get instruction for a specified Byte

			if(RegPrefix==0) // no prefix
			{   
				// read 4 bytes into EAX
				SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
                sprintf(menemonic,"%s %s, %08X",mene,Regs[REG32][0],dwMem);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %08X",Op,dwOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else if(RegPrefix==1) // RegPrefix is being used
			{   
				// read 2 bytes into AX (REG16)
				SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
                sprintf(menemonic,"%s %s, %04X",mene,Regs[REG16][0],wMem);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %04X",Op,wOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}
        }
        break;

        case 0x06: // PUSH ES
        {
            strcat(Disasm->Assembly,"push es");
            strcpy(Disasm->Remarks,"Push ES register to the stack");            
			strcat(Disasm->Opcode,"06");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x07: // POP ES
        {
            strcat(Disasm->Assembly,"pop es");
            strcpy(Disasm->Remarks,"Pop top stack to ES");            
			strcat(Disasm->Opcode,"07");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x0E: // PUSH CS
        {
            strcat(Disasm->Assembly,"push cs");
            strcpy(Disasm->Remarks,"Push CS register to the stack");
			strcat(Disasm->Opcode,"0E");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

        // INTEL - NEW SET (MMX/3DNow!/SSE/SSE2)
        case 0x0F:
        {
          char Instruction[128],m_bytes[128];
          int RetVal;
          BYTE Code=(BYTE)Opcode[i+1];

          strcat(Disasm->Opcode,"0F");
          RetVal=GetNewInstruction(Code,Instruction,RegPrefix);

          switch(RetVal) // check if we need to decode instruction
          {
            case 0:
            {
                // Decode SIB + ModRM
                if((BYTE)Opcode[i+2]>=0x00 && (BYTE)Opcode[i+2]<=0xBF)
                {
                    (*Index)++;
                    i=*Index;
                    Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
				    Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
                    Mod_RM_SIB_EX(&Disasm,&Opcode,i,AddrPrefix,SEG,&Index,Code,RegPrefix,SegPrefix,AddrPrefix,Bit_D,Bit_W,RepPrefix);
                    Disasm->PrefixSize=PrefixesSize;
                    Disasm->OpcodeSize++; // 0F extra Byte
                    break;
			    }
                else
                {
                    if(((BYTE)Opcode[i+2] & 0xC0)==0xC0)
                    {                    
                        Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
                        Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
                        (*Index)++;
                        i=*Index;
                        Mod_11_RM_EX(Bit_D,Bit_W,&Opcode,&Disasm,RegPrefix,Code,&Index,RepPrefix); // Decode with bits
                        Disasm->PrefixSize=PrefixesSize;
                        Disasm->OpcodeSize++; // 0F extra Byte
                    }
                    break;
                }
            }
            break; // big set instructions
            
            case 1: // 1 byte instructions set
            {
                strcat(Disasm->Assembly,Instruction);
                sprintf(Instruction,"%02X",Code);
                strcat(Disasm->Opcode,Instruction);
                Disasm->OpcodeSize=2;
                Disasm->PrefixSize=PrefixesSize;
                (*Index)++;
            }
            break;
            
            case 2: // NEAR JUMP (JXX)
            {
                SwapDword((BYTE*)(Opcode+i+2),&dwOp,&dwMem);
                dwMem+=Disasm->Address+PrefixesSize+6; // calculate dest addr
                sprintf(m_bytes,"%08X",dwMem);
                strcat(Instruction,m_bytes);
                strcat(Disasm->Assembly,Instruction);

                sprintf(m_bytes,"%08X",dwOp);
                sprintf(Instruction,"%02X ",Code);
                strcat(Disasm->Opcode,Instruction);
                strcat(Disasm->Opcode,m_bytes);       
                Disasm->OpcodeSize=6;
                Disasm->PrefixSize=PrefixesSize;
                (*Index)+=5;
                
            }
            break; // jump instructions set

            case 3:
            {
                if(((BYTE)Opcode[i+2]&0xC0)==0xC0)
                {
                    Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
                    Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
                    (*Index)++;
                    i=*Index;
                    Mod_11_RM_EX(Bit_D,Bit_W,&Opcode,&Disasm,RegPrefix,Code,&Index,RepPrefix); // Decode with bits
                    Disasm->PrefixSize=PrefixesSize;
                    Disasm->OpcodeSize++; // 0F extra Byte
                }
                else
                {
                    strcat(Disasm->Assembly,Instruction);
                    sprintf(Instruction,"%02X",Code);
                    strcat(Disasm->Opcode,Instruction);
                    Disasm->OpcodeSize=2;
                    Disasm->PrefixSize=PrefixesSize;
                    (*Index)++;
                }
            }
            break;

			case 4:
			{
				if( ((BYTE)Opcode[i+2]>=0x08) && ((BYTE)Opcode[i+2]<=0x0F) )
				{
                    (*Index)++;
                    i=*Index;
                    Bit_D=(Op&0x02)>>1;      // Get bit d (direction)
				    Bit_W=(Op&0x01);         // Get bit w (full/partial reg size)
                    Mod_RM_SIB_EX(&Disasm,&Opcode,i,AddrPrefix,SEG,&Index,Code,RegPrefix,SegPrefix,AddrPrefix,Bit_D,Bit_W,RepPrefix);
                    Disasm->PrefixSize=PrefixesSize;
                    Disasm->OpcodeSize++; // 0F extra Byte
				}
				else{
					strcat(Disasm->Assembly,"???");
					sprintf(Instruction,"%02X",Code);
					strcat(Disasm->Opcode,Instruction);
					Disasm->OpcodeSize=2;
					Disasm->PrefixSize=PrefixesSize;
					(*Index)++;
				}
			}
			break;
          }
          
          
        }
        break;

		case 0x16: // PUSH SS
        {
            strcat(Disasm->Assembly,"push ss");
            strcpy(Disasm->Remarks,"Push SS register to the stack");            
			strcat(Disasm->Opcode,"16");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x17: // POP SS
        {
            strcat(Disasm->Assembly,"pop ss");
            strcpy(Disasm->Remarks,"Pop top stack to SS");            
			strcat(Disasm->Opcode,"17");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x1E: // PUSH DS
        {
            strcat(Disasm->Assembly,"push ds");
            strcpy(Disasm->Remarks,"Push DS register to the stack");            
			strcat(Disasm->Opcode,"1E");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x1F: // POP DS
        {
            strcat(Disasm->Assembly,"pop ds");
            strcpy(Disasm->Remarks,"Pop top stack to DS"); 
			strcat(Disasm->Opcode,"1F");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x27: // DAA
        {
            strcat(Disasm->Assembly,"daa");
            strcat(Disasm->Opcode,"27");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x2F: // DAS
        {
            strcat(Disasm->Assembly,"das");
            strcat(Disasm->Opcode,"2F");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x37: // AAA
        {
            strcat(Disasm->Assembly,"aaa");
            strcat(Disasm->Opcode,"37");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x3F: // AAS
        {
            strcat(Disasm->Assembly,"aas");
            strcat(Disasm->Opcode,"3F");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x40:case 0x41: // INC XXX/XX
		case 0x42:case 0x43: // INC XXX/XX
		case 0x44:case 0x45: // INC XXX/XX
		case 0x46:case 0x47: // INC XXX/XX
        {
			sprintf(menemonic,"inc %s",Regs[RM][Op&0x0F]); // Find reg by Masking (Op&0x0F)
            strcat(Disasm->Assembly,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x48:case 0x49: // DEC XXX/XX
		case 0x4A:case 0x4B: // DEC XXX/XX
		case 0x4C:case 0x4D: // DEC XXX/XX
		case 0x4E:case 0x4F: // DEC XXX/XX
        {
			sprintf(menemonic,"dec %s",Regs[RM][Op&0x0F-0x08]);// Find reg by Masking (Op&0x0F-0x08)
            strcat(Disasm->Assembly,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x50:case 0x51: // PUSH XXX/XX
		case 0x52:case 0x53: // PUSH XXX/XX
		case 0x54:case 0x55: // PUSH XXX/XX
		case 0x56:case 0x57: // PUSH XXX/XX
        {
			sprintf(menemonic,"push %s",Regs[RM][Op&0x0F]);// Find reg by Masking (Op&0x0F)
            strcat(Disasm->Assembly,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x58:case 0x59: // POP XXX/XX
		case 0x5A:case 0x5B: // POP XXX/XX
		case 0x5C:case 0x5D: // POP XXX/XX
		case 0x5E:case 0x5F: // POP XXX/XX
        {
			sprintf(menemonic,"pop %s",Regs[RM][(Op&0x0F)-0x08]);// Find reg by Masking (Op&0x0F-0x08)
            strcat(Disasm->Assembly,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x60: // PUSHAD/W (Prefix)
        {
			if(!RegPrefix) // if RegPrefix == 0
				strcat(Disasm->Assembly,"pushad");
			else if(RegPrefix==1)// Change Reg Size
				strcat(Disasm->Assembly,"pushaw");
            
            strcat(Disasm->Opcode,"60");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x61: // POPAD/W (Prefix) 
        {
			if(!RegPrefix) // if RegPrefix == 0
				strcat(Disasm->Assembly,"popad");
			else if(RegPrefix==1)// Change Reg Size
				strcat(Disasm->Assembly,"popaw");
            
            strcat(Disasm->Opcode,"61");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x68: // PUSH XXXXXXXX
        {
			if(RegPrefix==0)
			{   // PUSH 4 bytes
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
				sprintf(menemonic,"push %08X",dwMem);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"68 %08X",dwOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else 
			{
				// PUSH 2 bytes
				SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
                sprintf(menemonic,"push %04X",wMem);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"68 %04X",wOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}
        }
        break;

		case 0x6A: // PUSH XX
        {
			if((BYTE)Opcode[i+1]>=0x80) // Signed Numebers (Negative)
				sprintf(menemonic,"push -%02X",(0x100-(BYTE)Opcode[i+1]));
			else
				sprintf(menemonic,"push %02X",(BYTE)Opcode[i+1]); // Unsigned Numbers (Positive)
            strcat(Disasm->Assembly,menemonic);
            sprintf(menemonic,"6A%02X",(BYTE)*(Opcode+i+1));
            strcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

		case 0x6C: case 0x6D: // INSB/INSW/INSD
        {
			if((Op&0x0F)==0x0C)
			{
				strcat(Disasm->Assembly,"insb");
				sprintf(menemonic,"Byte ptr ES:[%s], DX",Regs[ADDRM][7]);
				strcpy(Disasm->Remarks,menemonic);
			}
			else  if((Op&0x0F)==0x0D)
					if(!RegPrefix) // If RegPrefix == 0
					{
						strcat(Disasm->Assembly,"insd");
						sprintf(menemonic,"Dword ptr ES:[%s], DX",Regs[ADDRM][7]);
						strcpy(Disasm->Remarks,menemonic);
					}
					else if(RegPrefix==1) // Found RegPrefix == 1
						{
							strcat(Disasm->Assembly,"insw");
							sprintf(menemonic,"Word ptr ES:[%s], DX",Regs[ADDRM][7]);
							strcpy(Disasm->Remarks,menemonic);
						}

			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x6E: case 0x6F: // OUTSB/OUTSW/OUTSD
        {
			if((Op&0x0F)==0x0E)
			{
				strcat(Disasm->Assembly,"outsb");
				sprintf(menemonic,"DX, Byte ptr ES:[%s]",Regs[ADDRM][7]);
				strcpy(Disasm->Remarks,menemonic);
			}
			else  if((Op&0x0F)==0x0F)
					if(!RegPrefix) // If RegPrefix == 0
					{
						strcat(Disasm->Assembly,"outsd");
						sprintf(menemonic,"DX, Dword ptr ES:[%s]",Regs[ADDRM][7]);
						strcpy(Disasm->Remarks,menemonic);
					}
					else if(RegPrefix==1) // Found RegPrefix == 1
						{
							strcat(Disasm->Assembly,"outsw");
							sprintf(menemonic,"DX, Word ptr ES:[%s]",Regs[ADDRM][7]);
							strcpy(Disasm->Remarks,menemonic);
						}

			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x70: case 0x71: case 0x72: case 0x73: //JUMP XXXXXXXX
		case 0x74: case 0x75: case 0x76: case 0x77: //JUMP XXXXXXXX 
		case 0x78: case 0x79: case 0x7A: case 0x7B: //JUMP XXXXXXXX
		case 0x7C: case 0x7D: case 0x7E: case 0x7F: //JUMP XXXXXXXX
		case 0xE0: case 0xE1: case 0xE2: case 0xEB: //JUMP XXXXXXXX
        case 0xE3:
        {
			DWORD JumpAddress=0;
			BYTE JumpSize;
			char temp[10];
			JumpSize=(BYTE)Opcode[i+1];

            // Short Jump $+2
			if((BYTE)Opcode[i+1]>0x7F)
				JumpAddress=Disasm->Address + ((2 + PrefixesSize + JumpSize)-0x100);
			else
				JumpAddress=Disasm->Address + 2 + JumpSize  +PrefixesSize;
			
			GetJumpInstruction(Op,temp);
            if(Op==0xE3 && AddrPrefix==1)
			     strcpy(temp,"jcxz");

            sprintf(menemonic,"%s %08X",temp,JumpAddress);
			strcat(Disasm->Assembly,menemonic);
			sprintf(menemonic,"%02X%02X",Op,(BYTE)Opcode[i+1]);
			strcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
			++(*Index);		
        }
        break;

		case 0x90: // NOP (XCHG EAX, EAX) 
        {
            strcat(Disasm->Assembly,"nop");
            strcat(Disasm->Opcode,"90");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x91:case 0x92: // XCHG XXX, XXX
		case 0x93:case 0x94: // XCHG XXX, XXX
		case 0x95:case 0x96: // XCHG XXX, XXX
		case 0x97:           // XCHG XXX, XXX
		{
			Mod_11_RM(1,1,&Opcode,&Disasm,"xchg",RegPrefix,Op,&Index);//+ 0x30			
			Disasm->PrefixSize=PrefixesSize;
		}
		break;

		case 0x98: // CWDE/CDW (Prefix) 
        {
			if(!RegPrefix)
				strcat(Disasm->Assembly,"cwde");
			else if(RegPrefix==1)
				strcat(Disasm->Assembly,"cbw");

            strcat(Disasm->Opcode,"98");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x99: // CWDE/CDW (Prefix) 
        {
			if(!RegPrefix)
				strcat(Disasm->Assembly,"cdq");
			else if(RegPrefix==1)
				strcat(Disasm->Assembly,"cwd");

            strcat(Disasm->Opcode,"98");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9A: case 0xEA: // CALL/JMP XXXX:XXXXXXXX (FAR CALL)
		{
			char temp[10];

			switch(Op)
			{
				case 0x9A:strcpy(temp,"call");break;
				case 0xEA:strcpy(temp,"jmp");break;
			}

			if(AddrPrefix==0)
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
                SwapWord((BYTE*)(Opcode+i+5),&wOp,&wMem);

				sprintf(menemonic,"%s %04X:%08X",temp,wMem,dwMem);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %08X %04X",Op,dwOp,wOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=7;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=6;
			}
			else
			{
                WORD w_op,w_mem;
                SwapWord((BYTE*)(Opcode+i+3),&wOp,&wMem);
                SwapWord((BYTE*)(Opcode+i+1),&w_op,&w_mem);  
                
				sprintf(menemonic,"%s %04X:%08X",temp,wMem,w_mem);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %04X %04X",Op,w_op,wOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			sprintf(menemonic,"Far %s",temp);
			strcpy(Disasm->Remarks,menemonic);
			
		}
		break;

		case 0x9B: // WAIT
        {
            strcat(Disasm->Assembly,"wait");
            strcat(Disasm->Opcode,"9B");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9C: // PUSHFD/PUSHFW 
        {
			if(!RegPrefix)
				strcat(Disasm->Assembly,"pushfd");
			else if(RegPrefix==1)
				strcat(Disasm->Assembly,"pushfw");

            strcat(Disasm->Opcode,"9C");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9D: // POPFD/POPFW 
        {
			if(!RegPrefix)
				strcat(Disasm->Assembly,"popfd");
			else if(RegPrefix==1)
				strcat(Disasm->Assembly,"popfw");

            strcat(Disasm->Opcode,"9D");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9E: // SAHF
        {
            strcat(Disasm->Assembly,"sahf");
            strcat(Disasm->Opcode,"9E");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0x9F: // LAHF
        {
            strcat(Disasm->Assembly,"lahf");
            strcat(Disasm->Opcode,"9F");            
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xA0:case 0xA2: // MOV AL, BYTE PTR XX:[XXXXXXXX], AL
		{
			if(!AddrPrefix)
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
				switch(Op)
				{
					case 0xA0:sprintf(menemonic,"mov al, Byte ptr %s:[%08X]",Segs[SEG],dwMem);break;
					case 0xA2:sprintf(menemonic,"mov Byte ptr %s:[%08X], al",Segs[SEG],dwMem);break;
				}

				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %08X",Op,dwOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else 
			{
                SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
				switch(Op)
				{
					case 0xA0:sprintf(menemonic,"mov al, Byte ptr %s:[%04X]",Segs[SEG],wMem);break;
					case 0xA2:sprintf(menemonic,"mov Byte ptr %s:[%04X], al",Segs[SEG],wMem);break;
				}
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %04X",Op,wOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}
		}
		break;

		case 0xA1:case 0xA3: // MOV EAX/AX, BYTE PTR XX:[XXXXXXXX], EAX/AX
		{
			if(!AddrPrefix)// no addr size change
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
				switch(Op)
				{
					case 0xA1:sprintf(menemonic,"mov %s, %s ptr %s:[%08X]",Regs[RM][0],RSize,Segs[SEG],dwMem);break;
					case 0xA3:sprintf(menemonic,"mov %s ptr %s:[%08X], %s",RSize,Segs[SEG],dwMem,Regs[RM][0]);break;
				}
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %08X",Op,dwOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else if(AddrPrefix==1)
			{
                SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
				switch(Op) // change addr size DWORD->WORD
				{
					case 0xA1:sprintf(menemonic,"mov %s, %s ptr %s:[%04X]",Regs[RM][0],RSize,Segs[SEG],wMem);break;
					case 0xA3:sprintf(menemonic,"mov %s ptr %s:[%04X], %s",RSize,Segs[SEG],wMem,Regs[RM][0]);break;
				}
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %04X",Op,wOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}						
		}
		break;

        case 0xA4:case 0xA5: // MOVSB/MOVSW/MOVSD
        {
			if(RepPrefix==1 && (BYTE)Opcode[i-1]==0xF3)
				strcpy(Disasm->Assembly,"rep ");

			if((Op&0x0F)==0x04)
				sprintf(menemonic,"Byte ptr %s:[%s], Byte ptr %s:[%s]",Segs[SEG_ES],Regs[ADDRM][7],Segs[SEG],Regs[ADDRM][6]);
			else if((Op&0x0F)==0x05)
				  sprintf(menemonic,"%s ptr %s:[%s], %s ptr %s:[%s]",RSize,Segs[SEG_ES],Regs[ADDRM][7],RSize,Segs[SEG],Regs[ADDRM][6]);
			
			strcat(Disasm->Assembly,"movs");
            strcpy(Disasm->Remarks,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xA6:case 0xA7: // CMPSB/CMPSW/CMPSD
        {
			if((Op&0x0F)==0x06)
			{
				if(RepPrefix==1)
					sprintf(menemonic,"Byte ptr %s:[%s], Byte ptr %s:[%s]",Segs[SEG_ES],Regs[ADDRM][7],Segs[SEG],Regs[ADDRM][6]);
				else 
					sprintf(menemonic,"Byte ptr %s:[%s], Byte ptr %s:[%s]",Segs[SEG],Regs[ADDRM][6],Segs[SEG_ES],Regs[ADDRM][7]);
			}
			else if((Op&0x0F)==0x07)
			{
				if(RepPrefix==1)
				  sprintf(menemonic,"%s ptr %s:[%s], %s ptr %s:[%s]",RSize,Segs[SEG_ES],Regs[ADDRM][7],RSize,Segs[SEG],Regs[ADDRM][6]);
				else
				  sprintf(menemonic,"%s ptr %s:[%s], %s ptr %s:[%s]",RSize,Segs[SEG],Regs[ADDRM][6],RSize,Segs[SEG_ES],Regs[ADDRM][7]);
			}
			
			strcat(Disasm->Assembly,"cmps");
            strcpy(Disasm->Remarks,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xAA:case 0xAB: // STOSB/STOSW/STOSD
        {
			if(RepPrefix==1 && (BYTE)Opcode[i-1]==0xF3)
				strcpy(Disasm->Assembly,"rep ");

			if((Op&0x0F)==0x0A)
				sprintf(menemonic,"Byte ptr %s:[%s]",Segs[SEG_ES],Regs[ADDRM][7]);
			else if((Op&0x0F)==0x0B)
				  sprintf(menemonic,"%s ptr %s:[%s]",RSize,Segs[SEG_ES],Regs[ADDRM][7]);
			
			strcat(Disasm->Assembly,"stos");
            strcpy(Disasm->Remarks,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xAC:case 0xAD: // LODSB/LODSW/LODSD
        {
			if(RepPrefix==1 && (BYTE)Opcode[i-1]==0xF3)
				strcpy(Disasm->Assembly,"rep ");

			if((Op&0x0F)==0x0C)
				sprintf(menemonic,"Byte ptr %s:[%s]",Segs[SEG_DS],Regs[ADDRM][6]);
			else if((Op&0x0F)==0x0D)
				  sprintf(menemonic,"%s ptr %s:[%s]",RSize,Segs[SEG_DS],Regs[ADDRM][6]);
			
			strcat(Disasm->Assembly,"lods");
            strcpy(Disasm->Remarks,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xAE:case 0xAF: // SCASB/SCASW/SCASD
        {
			if((Op&0x0F)==0x0E)
				sprintf(menemonic,"Byte ptr %s:[%s]",Segs[SEG_ES],Regs[ADDRM][7]);
			else if((Op&0x0F)==0x0F)
				  sprintf(menemonic,"%s ptr %s:[%s]",RSize,Segs[SEG_ES],Regs[ADDRM][7]);
			
			strcat(Disasm->Assembly,"scas");
            strcpy(Disasm->Remarks,menemonic);
			sprintf(menemonic,"%02X",Op);
            strcat(Disasm->Opcode,menemonic);            
			Disasm->PrefixSize=PrefixesRSize;
        }
        break;

		case 0xB0:case 0xB1: // MOV XX, XX
	    case 0xB2:case 0xB3: // MOV XX, XX
		case 0xB4:case 0xB5: // MOV XX, XX
		case 0xB6:case 0xB7: // MOV XX, XX
        {
            sprintf(menemonic,"mov %s, %02X",Regs[REG8][Op&0xF],(BYTE)Opcode[i+1]);
            strcat(Disasm->Assembly,menemonic);
            sprintf(menemonic,"%02X%02X",Op,(BYTE)*(Opcode+i+1));
            strcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

		case 0xB8:case 0xB9: // MOV XX/XXX, XXXXXXXX
		case 0xBA:case 0xBB: // MOV XX/XXX, XXXXXXXX
		case 0xBC:case 0xBD: // MOV XX/XXX, XXXXXXXX
		case 0xBE:case 0xBF: // MOV XX/XXX, XXXXXXXX
		{
			if(!RegPrefix) // check if default prefix has changed
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);
				sprintf(menemonic,"mov %s, %08X",Regs[RM][(Op&0xF)-0x08],dwMem);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %08X",Op,dwOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=4;
			}
			else
			{
                SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
				sprintf(menemonic,"mov %s, %04X",Regs[RM][(Op&0xF)-0x08],wMem);
				strcat(Disasm->Assembly,menemonic);    
                sprintf(menemonic,"%02X %04X",Op,wOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize=PrefixesSize;
				(*Index)+=2;
			}
		}
		break;

		case 0xC2:case 0xCA: // RET/F XXXX
		{
			char code[6];
			switch(Op)
			{
		 	    case 0xC2:sprintf(code,"ret");break;
				case 0xCA:sprintf(code,"retf");break;
			}
            
            SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
            if(wMem>=0xA000)
              sprintf(menemonic,"%s %05X",code,wMem);
            else
                sprintf(menemonic,"%s %04X",code,wMem);
			
			strcat(Disasm->Assembly,menemonic);
			sprintf(menemonic,"%02X %04X",Op,wOp);
			strcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=3;
			Disasm->PrefixSize=PrefixesSize;
			if(LockPrefix==1)
				strcat(Disasm->Remarks,"<Illegal Lock Prefix>");
			
			(*Index)+=2;
		}
		break;

		case 0xC3: // RET
        {
            strcat(Disasm->Assembly,"ret");
            strcat(Disasm->Opcode,"C3");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"Pop IP");
        }
        break;

		case 0xC8: // ENTER XXXX, XX
		{
            SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);
			sprintf(menemonic,"enter %04X, %02X",wMem,(BYTE)Opcode[i+3]);
			strcat(Disasm->Assembly,menemonic);
			sprintf(menemonic,"C8 %04X %02X",wOp,(BYTE)Opcode[i+3]);
			strcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=4;
			Disasm->PrefixSize=PrefixesSize;
			(*Index)+=3;
        }
		break;

		case 0xC9: // LEAVE
        {
            strcat(Disasm->Assembly,"leave");
            strcat(Disasm->Opcode,"C9");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xCB: // RETF
        {
            strcat(Disasm->Assembly,"retf");
            strcat(Disasm->Opcode,"CB");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xCC: // INT 3
        {
            strcat(Disasm->Assembly,"int3");
            strcat(Disasm->Opcode,"CC");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xCD: // INT XX
        {
			sprintf(menemonic,"int %02X",(BYTE)Opcode[i+1]);
            strcat(Disasm->Assembly,menemonic);
            sprintf(menemonic,"CD %02X",(BYTE)*(Opcode+i+1));
            strcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

		case 0xCE: // INTO
        {
            strcat(Disasm->Assembly,"into");
            strcat(Disasm->Opcode,"CE");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xCF: // IRETD/W
        {
			if(!RegPrefix){
				strcat(Disasm->Assembly,"iretd");
			}
			else if(RegPrefix==1){
				strcat(Disasm->Assembly,"iretw");
			}
            strcat(Disasm->Opcode,"CF");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xD4:case 0xD5: // AAM/AAD
        {
			char opcode[5];
			switch(Op)
			{
				case 0xD4:sprintf(opcode,"aam");break;
				case 0xD5:sprintf(opcode,"aad");break;
			}
			sprintf(menemonic,"%s %02X",opcode,(BYTE)Opcode[i+1]);
            strcat(Disasm->Assembly,menemonic);
            sprintf(menemonic,"%02X%02X",Op,(BYTE)*(Opcode+i+1));
            strcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
            ++(*Index);
        }
        break;

		case 0xD6: // SALC
        {
            strcat(Disasm->Assembly,"salc");
            strcat(Disasm->Opcode,"D6");
			Disasm->PrefixSize=PrefixesSize;
        }
        break;

		case 0xD7: // XLAT
		{
			strcat(Disasm->Assembly,"xlat");
			strcat(Disasm->Opcode, "D7");
			sprintf(menemonic,"Byte ptr %s:[%s+al]",Segs[SEG],Regs[ADDRM][3]);
			strcat(Disasm->Remarks,menemonic);
			Disasm->PrefixSize=PrefixesSize;
		}
		break;

		// 0xE4 is at 'in al, xxx' cases
		case 0xE5: // IN EAX/AX, XX
        {
			// special case Opcode, insted of reading DWORD (4 bytes), we read 1 BYTE.
			char mene[10]="";
			GetInstruction(Op,mene); // get instruction from opcode

			if(RegPrefix==0) // no prefix
			{   
				// read 4 bytes into EAX
				sprintf(menemonic,"%s %s, %02X",mene,Regs[REG32][0],(BYTE)Opcode[i+1]);
				strcat(Disasm->Assembly,menemonic);
			}
			else if(RegPrefix==1) // prefix is being used
			{   
				// read 2 bytes into AX
				sprintf(menemonic,"%s %s, %02X",mene,Regs[REG16][0],(BYTE)Opcode[i+1]);
				strcat(Disasm->Assembly,menemonic);
			}
			sprintf(menemonic,"%02X%02X",Op,(BYTE)Opcode[i+1]);
			strcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"I/O Instruction");
			++(*Index);
        }
        break;

		case 0xE6: // OUT XX, AL
        {
			char mene[10]="";
			GetInstruction(Op,mene);
            sprintf(menemonic,"%s %02X, al",mene,(BYTE)Opcode[i+1]);
            strcat(Disasm->Assembly,menemonic);
            strcpy(Disasm->Remarks,"");
            sprintf(menemonic,"%02X%02X",Op,(BYTE)*(Opcode+i+1));
            strcat(Disasm->Opcode,menemonic);
            Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"I/O Instruction");
            ++(*Index);
        }
        break;

		case 0xE7: // OUT XX, AX/EAX
        {
			// special case Opcode, insted of reading DWORD (4 bytes), we read 1 BYTE.
			char mene[10]="";
			GetInstruction(Op,mene); // get instruction from opcode

			if(RegPrefix==0) // no prefix
			{   
				// read 1 byte into EAX
				sprintf(menemonic,"%s %02X, %s",mene,(BYTE)Opcode[i+1],Regs[REG32][0]);
				strcat(Disasm->Assembly,menemonic);
			}
			else if(RegPrefix==1) // prefix is being used
			{   
				// read 1 byte into AX
				sprintf(menemonic,"%s %02X, %s",mene,(BYTE)Opcode[i+1],Regs[REG16][0]);
				strcat(Disasm->Assembly,menemonic);
			}
			sprintf(menemonic,"%02X%02X",Op,(BYTE)Opcode[i+1]);
			strcat(Disasm->Opcode,menemonic);
			Disasm->OpcodeSize=2;
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"I/O Instruction");
			++(*Index);
        }
        break;

		case 0xE8:case 0xE9: // CALL/JMP XXXX/XXXXXXXX
		{
			DWORD CallAddress=0;
			DWORD CallSize=0;
			char temp[10];

			switch(Op)
			{
				case 0xE8:strcpy(temp,"call");break;
				case 0xE9:strcpy(temp,"jmp");break;
			}

			if(!RegPrefix)
			{
                SwapDword((BYTE*)(Opcode+i+1),&dwOp,&dwMem);				
				dwMem+= Disasm->Address + CallSize + 5 + (PrefixesSize-RegPrefix);
				sprintf(menemonic,"%s %08X",temp,dwMem);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %08X",Op,dwOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=5;
				Disasm->PrefixSize = PrefixesSize;                
				(*Index)+=4;
			}
			else 
			{
                SwapWord((BYTE*)(Opcode+i+1),&wOp,&wMem);                
				if(wMem>=0x0000F000)
					CallAddress = (wMem + 4 + (PrefixesSize-RegPrefix))-0x0000F000;
				else
					CallAddress = (Disasm->Address-0x00400000) + wMem + 4 + (PrefixesSize-RegPrefix);
				sprintf(menemonic,"%s %08X",temp, CallAddress);
				strcat(Disasm->Assembly,menemonic);
				sprintf(menemonic,"%02X %04X",Op,wOp);
				strcat(Disasm->Opcode,menemonic);
				Disasm->OpcodeSize=3;
				Disasm->PrefixSize = PrefixesSize;
				(*Index)+=2;
			}
		}
		break;

		case 0xEC: // IN AL, DX
        {
            strcat(Disasm->Assembly,"in al, dx");
            strcat(Disasm->Opcode,"EC");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"I/O Instruction");
        }
        break;

		case 0xED: // IN AX/EAX, DX
        {
			sprintf(menemonic,"in %s, dx",Regs[RM][0]);
			strcat(Disasm->Assembly,menemonic);
            strcat(Disasm->Opcode,"ED");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"I/O Instruction");
        }
        break;

		case 0xEE: // OUT DX, AL
        {
            strcat(Disasm->Assembly,"out dx, al");
            strcat(Disasm->Opcode,"EE");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"I/O Instruction");
        }
        break;

		case 0xEF: // OUT DX, AX/EAX
        {
			sprintf(menemonic,"out dx, %s",Regs[RM][0]);
			strcat(Disasm->Assembly,menemonic);
            strcat(Disasm->Opcode,"EF");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"I/O Instruction");
        }
        break;

		case 0xF1: // ICEBP (INT1)
        {
			strcat(Disasm->Assembly,"int1");
            strcat(Disasm->Opcode,"F1");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"(icebp)");
        }
        break;

		case 0xF4: // HLT (HALT)
        {
			strcat(Disasm->Assembly,"hlt");
            strcat(Disasm->Opcode,"F4");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"Halts CPU until RESET");
        }
        break;

		case 0xF5: // CMC
        {
			strcat(Disasm->Assembly,"cmc");
            strcat(Disasm->Opcode,"F4");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"inverts the Carry Flag");
        }
        break;

		case 0xF8: // CLC
        {
			strcat(Disasm->Assembly,"clc");
            strcat(Disasm->Opcode,"F8");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"Clears the Carry Flag");
        }
        break;

		case 0xF9: // STC
        {
			strcat(Disasm->Assembly,"stc");
            strcat(Disasm->Opcode,"F9");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"Sets the Carry Flag to 1");
        }
        break;

		case 0xFA: // CLI
        {
			strcat(Disasm->Assembly,"cli");
            strcat(Disasm->Opcode,"FA");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"Set Interrupt flag to 0");
        }
        break;

		case 0xFB: // STI
        {
			strcat(Disasm->Assembly,"sti");
            strcat(Disasm->Opcode,"FB");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"Set Interrupt flag to 1");
        }
        break;

		case 0xFC: // CLD
        {
			strcat(Disasm->Assembly,"cld");
            strcat(Disasm->Opcode,"FC");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"Set Direction Flag to 0");
        }
        break;

		case 0xFD: // STD
        {
			strcat(Disasm->Assembly,"std");
            strcat(Disasm->Opcode,"FD");
			Disasm->PrefixSize=PrefixesSize;
			strcat(Disasm->Remarks,"Set Direction Flag to 1");
        }
        break;
    }
}

void ShowDecoded(DISASSEMBLY Disasm)
{
	// Printed all information regarding a 
	// Decoded instruction

    char space[]="      ";
    printf("%08X: %s %s %s /%d ; %s\n",Disasm.Address,
			                           Disasm.Opcode,
									   space,
									   Disasm.Assembly,
									   Disasm.OpcodeSize+Disasm.PrefixSize,
									   Disasm.Remarks
          );
}

void FlushDecoded(DISASSEMBLY *Disasm)
{
	// Clear all information of an decoded 
	// Instruction

    strcpy(Disasm->Assembly,""); // Clear menemonic
    strcpy(Disasm->Remarks,"");  // Clear commets
    strcpy(Disasm->Opcode,"");   // Clear opcodes linear
    Disasm->OpcodeSize=1;        // Smallest opcode size
	Disasm->PrefixSize=0;        // No Prefixes
}


/*
 *	---- Example ---
 */
/*
void main()
{
    DISASSEMBLY Disasm; // Creates an Disasm Struct
	// Linear Address to be decoded
    BYTE Opcodes[BYTES_TO_DECODE]={0xFF,0x58,0x00,0x00,0x00,0x00,0xdf,0x00,0x01,0x66,0x04,0x60,0x88,0xF7,0x12,0x13};
    // Pointer to linear address
	char *Linear="";
	// Index of opcoded to decode
    DWORD Index=0;
              
    Linear=(char*)Opcodes;     // Points to the address of array to decode.
    Disasm.Address=0x00401000; // Common Entry Point (usually default..)
    FlushDecoded(&Disasm);     // reset all content

    printf("Disassemble of Vector: ");
    for(Index=0;Index<BYTES_TO_DECODE;Index++)
        printf("%02X",(BYTE)Opcodes[Index]);
    
    printf("\n\n");
    
    for(Index=0;Index<BYTES_TO_DECODE;Index++)
    {
		// Decode instruction
        Decode(&Disasm,Linear,&Index);

		// Upper case Assembly (Optional)
        CharUpper(Disasm.Assembly);
		// Show Decoded instruction, size, remarks...
        ShowDecoded(Disasm);
		// Calculate total Size of an instruction + Prefixes, and
		// Fix the address of IP 
        Disasm.Address+=Disasm.OpcodeSize+Disasm.PrefixSize;
		// Clear all information
        FlushDecoded(&Disasm);
    }

    getch();
}

*/