/* IRLib_P13_IEEE.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * This is dummy code that you can copy and rename and modify when implementing new protocols.
 */

#ifndef IRLIB_PROTOCOL_13_H
#define IRLIB_PROTOCOL_13_H
#define IR_SEND_PROTOCOL_13		case 13: IRsendIEEE::send(data); break;
#define IR_DECODE_PROTOCOL_13	if(IRdecodeIEEE::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_13 ,public virtual IRdecodeIEEE
	#define PV_IR_SEND_PROTOCOL_13   ,public virtual IRsendIEEE
#else
	#define PV_IR_DECODE_PROTOCOL_13  public virtual IRdecodeIEEE
	#define PV_IR_SEND_PROTOCOL_13    public virtual IRsendIEEE
#endif

#ifdef IRLIBSENDBASE_H
class IRsendIEEE: public virtual IRsendBase {
  public:
    void IRsendIEEE::send(uint32_t data,uint8_t kHz=38) {
      if (data==REPEAT_CODE) {
        enableIROut(kHz);
        /*mark(560*16); space(560*8); //start burst & space
        mark(560);space(560); //logical 0s x 5
        mark(560);space(560);
        mark(560);space(560);
        mark(560);space(560);
        mark(560);space(560);*/
        //message


      } else {
        sendGeneric(data,3, 560*16, 560*8, 560, 560, 560*3, 560, kHz, true,108000);
      }
    };
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodeIEEE: public virtual IRdecodeBase {
  public:
    bool IRdecodeIEEE::decode(void) {
    resetDecoder();
      IRLIB_ATTEMPT_MESSAGE(F("IEEE"));
        if (recvGlobal.decodeLength == 4 && MATCH(recvGlobal.decodeBuffer[1],560*16) && MATCH(recvGlobal.decodeBuffer[2],560*4)
          		&& MATCH(recvGlobal.decodeBuffer[3],564)) {
            bits = 3;
            value = REPEAT_CODE;
            protocolNum = IEEE;
            return true;
          }
          IRLIB_ATTEMPT_MESSAGE(F("NEC"));
          if(!decodeGeneric(10, 560*16, 560*8, 560, 560*3, 560)) return false;
          protocolNum = IEEE;
          return true;
    }
};

#endif //IRLIBDECODEBASE_H

#define IRLIB_HAVE_COMBO

#endif //IRLIB_PROTOCOL_13_H
