#include "TFT.h"
#include "crntsc.h"

Tft TFT;

void __attribute__((aligned(64))) TC4_Handler()
{
  /*
  static int disp=0;
    long dummy=REG_TC1_SR1; 

    int c=REG_PWM_CCNT2;
    if(!TFT.synced)
      if((c<=TFT.xstart+1)&&(c>=TFT.xstart)){
	REG_PWM_CPRDUPD2=TFT.xclocks;
	TFT.synced=1;
      }
    if(disp==TFT_COLOUR){  
      REG_DMAC_CTRLA5=0x22060000 + (TFT.cw >> 2); 
      REG_DMAC_CHER=1<<5;   
    }    
    }    
    
                      
    if(TFT.mode==TFT_NTSC){
      int p;
      static uint16_t *buf;

      REG_DMAC_SADDR5=(int)buf;
      REG_DMAC_CTRLA5=0x22060000 + 221;
      REG_DMAC_CHER=1<<5;   
      buf=(uint16_t *)(((int)TFT.dmabuf)+((TFT.line&1)*1024));
      
      asm volatile(
	".rept 18 \n\t"
	" ldrh r0,[%[cbt]], #2 \n\t"
	" strh r0,[%[dbo]], #2 \n\t"
	".endr \n\t"
	:
	:[dbo]"r"(buf+41)
        ,[cbt]"r"(&TFT.cbt[0][TFT.phase + 41])
	:"r0"
      );

      if(TFT.line < TFT.ysize){
	p=TFT.phase + 88;if(p>=88)p-=88;
	asm volatile(
        " mov r0,#45056                    \n\t"
        " mov r1,#512                      \n\t"
        
        "1:                                \n\t"

        ".rept 8                           \n\t"
        " ldrb r2, [%[cbl]], #1           \n\t"
        " ldrh r3, [%[crt], r2, lsl #1]   \n\t"
        " strh r3, [%[dbo]], #2           \n\t" 
        " adds %[crt],r1                    \n\t"
        ".endr                             \n\t"
        
        " cmp %[crt],%[cre]                \n\t"
        " it gt                            \n\t"
        " subgt %[crt],r0                  \n\t"       
        
	".rept 8                           \n\t"
        " ldrb r2, [%[cbl]], #1           \n\t"
        " ldrh r3, [%[crt], r2, lsl #1]   \n\t"
        " strh r3, [%[dbo]], #2           \n\t" 
        " adds %[crt],r1                    \n\t"
        ".endr                             \n\t"
        
        " cmp %[crt],%[cre]                \n\t"
        " it gt                            \n\t"
        " subgt %[crt],r0                  \n\t"       
	
        " cmp %[cbl],%[cbe]                \n\t"
        " bne 1b                           \n\t"
        
        :
        :[dbo]"r"(buf+88)
        ,[cbl]"r"(TFT.cb+TFT.line*320)
        ,[cbe]"r"(TFT.cb+TFT.line*320+320)
        ,[crt]"r"(TFT.crt[0]+p*256)
        ,[cre]"r"(TFT.crt[0]+88*256-1)
        :"r0","r1","r2","r3"
        );      	
	
      }
      else if ((TFT.line == TFT.ysize)||(TFT.line == TFT.ysize+1))
      {
	uint32_t *lp = (uint32_t *)(buf+88);
        for(int i=0;i<160;i++)*lp++=0x3c3c3c3c;
      }
      else if ((TFT.line == TFT.ysyncstart)||(TFT.line == TFT.ysyncstart+1))
      {
        uint32_t *lp = (uint32_t *)buf;
        for(int i=0;i<16;i++)*lp++=0x3c3c3c3c;
        for(int i=16;i<222;i++)*lp++=0;                
      }
      else if ((TFT.line == TFT.ysyncend)||(TFT.line == TFT.ysyncend+1))
      {
        uint32_t *lp = (uint32_t *)buf;
        for(int i=0;i<16;i++)*lp++=0;
        for(int i=16;i<222;i++)*lp++=0x3c3c3c3c;   		
      }
      TFT.phase+=TFT.poff;if(TFT.phase >= 88)TFT.phase -= 88;
      TFT.line++;if(TFT.line == TFT.ytotal)TFT.line=0;
      return;
    }

    if(TFT.line==TFT.ysyncstart) _v_digitalWriteDirect(_v_vsync, TFT.vsyncpol); 
    if(TFT.line==TFT.ysyncend) _v_digitalWriteDirect(_v_vsync,!TFT.vsyncpol); 
    TFT.linedouble++; 
    if(TFT.linedouble == TFT.yscale){TFT.linedouble=0;TFT.line++;}
    if(TFT.line == TFT.ysize)disp=0;
    if(TFT.line == TFT.ytotal){
      if(TFT.mode == TFT_MONO)REG_DMAC_SADDR4=(uint32_t)TFT.pb;
      else if(TFT.mode == TFT_COLOUR)REG_DMAC_SADDR5=(uint32_t)TFT.cb;
      TFT.line=0;disp=TFT.mode;TFT.framecount++;
    }
  */
}

void __attribute__((aligned(64))) PWM_Handler()
{
  /*
    long t=(REG_PWM_ISR1);
    if(TFT.linedouble){
      if(TFT.mode == TFT_MONO)REG_DMAC_SADDR4-=(TFT.pw<<1);
      else REG_DMAC_SADDR5-=(TFT.cw);
    }
    //TFT.debug=REG_TC0_CV1;
    asm volatile("wfe \n\t");
  */  
}

void __attribute__((aligned(64))) DMAC_Handler()
{
  /*
    REG_PIOA_PER  = 1<<26;     
    uint32_t dummy=REG_DMAC_EBCISR;
   */ 
}

int Tft::calcmodeline()
{
    //try to find a suitable modeline
  for (xscale = 16; xscale > 1; xscale--)
   for(int yti=0;yti<50;yti++){
    pclock = 84000000 / xscale;
    xtotal = (xsize * 5 / 4);
    if(xtotal & 1) xtotal -= 1;
    xsyncstart=((10*xsize+2*xtotal)/12);
    xsyncend=((5*xsize+7*xtotal)/12);
    ytotal = ((ysize*25)/24)+yti;
    ysyncstart=((10*ysize+2*ytotal)/12)+1;
    ysyncend=((8*ysize+4*ytotal)/12)+1;
    if(ysyncstart <= ysize)ysyncstart=ysize+1;
    if(ysyncend <= ysyncstart)ysyncend=ysyncstart+1;
        
    lfreq = pclock / xtotal;
    for(yscale=1;yscale<=8;yscale++){
        ffreq = lfreq / (ytotal * yscale);
        ltot = ysize * yscale;
	if ((lfreq > lfreqmin) && (lfreq < lfreqmax) && 
	    (ffreq > ffreqmin) && (ffreq < ffreqmax))
	      goto foundmode;
    }
  }
  foundmode:;
  if(xscale==1)return -1;
  if(mode == TFT_COLOUR && xscale < 6)return -1;
    
  // calculate timings from modeline data
  xclocks=(xtotal*xscale) &~ 1;
  xstart=(xtotal - xsyncend)*xscale - 78;
  if(xstart < 132)xstart = 132;
  xsyncwidth = (xsyncend - xsyncstart)*xscale;
  
  return 0;
}

int Tft::allocvideomem()
{
    if(mode==TFT_MONO){
    pw=((xsize+31)/32)*2+2;
    pbsize=pw*ysize;
    pb=(uint16_t *)calloc(pbsize,2);
    if(pb==0)return -2;
    
    pbb=(uint32_t *)((int(pb-0x20000000)*32)+0x22000000);
    pbw=pw*16;
  }
  if((mode & TFT_COLOUR)){
    cw=xsize;
    cbsize=cw*ysize;
    cb=(uint8_t *)calloc(cbsize,1);
    if(cb==0)return -2;
  }
  return 0;
}

void Tft::freevideomem()
{
  if(pb){free(pb);pb=0;}
  if(cb){free(cb);cb=0;}  
}

void Tft::startinterrupts(){    /// TFT_F429_Discovery.h 와 충돌 
  
///  for(int i=0;i<45;i++)    NVIC_SetPriority(IRQn_Type (i),6);  
///  NVIC_SetPriority(DMAC_IRQn,4);
///  NVIC_SetPriority(UART_IRQn,5);
///  NVIC_SetPriority(PWM_IRQn,3);
///  NVIC_SetPriority(TC4_IRQn,1); 
///  NVIC_SetPriority(UOTGHS_IRQn,2); 
  
///  if(mode==TFT_MONO) NVIC_EnableIRQ(DMAC_IRQn);
///  NVIC_EnableIRQ(TC4_IRQn); 
///  NVIC_EnableIRQ(PWM_IRQn); 
}

void Tft::stopinterrupts(){  /// TFT_F429_Discovery.h 와 충돌 
///  NVIC_DisableIRQ(PWM_IRQn);
///  NVIC_DisableIRQ(TC4_IRQn);
///  NVIC_DisableIRQ(DMAC_IRQn); 
}

void Tft::starttimers(){    /// TFT_F429_Discovery.h 와 충돌 
 /* 
  REG_PIOA_PDR  =1<<20;
  REG_PIOA_ABSR|=1<<20;
  REG_PMC_PCER1= 1<<4;
  REG_PWM_WPCR= 0x50574dfc;
  REG_PWM_CLK= 0x00010001;
  REG_PWM_DIS= 1<<2;
  REG_PWM_CMR2=hsyncpol ? 0x0 : 0x200;
  REG_PWM_CPRD2=xclocks+1;
  REG_PWM_CDTY2=xclocks-xsyncwidth;
  REG_PWM_SCM=0;
  REG_PWM_IER1=1<<2;
  REG_PWM_ENA= 1<<2;  

  REG_PMC_PCER0= 1<<31;
  REG_TC1_WPMR=0x54494D00;
  REG_TC1_CMR1=0b00000000000010011100010000000000;
  REG_TC1_RC1=xclocks/2; 
  REG_TC1_RA1=0;  
  REG_TC1_CCR1=0b101;    
  REG_TC1_IER1=0b00010000; 
  REG_TC1_IDR1=0b11101111; 
 */ 
}

void Tft::stoptimers() {    /// TFT_F429_Discovery.h 와 충돌 
  /*
    REG_TC1_CCR1=0b10;    
    REG_TC1_IDR1=0b00010000; 
    REG_PMC_PCDR0= 1<<28;

    REG_PWM_DIS= 1<<2;
    REG_PWM_IDR1=1<<2;
  */
}

void Tft::startmono(){    /// TFT_F429_Discovery.h 와 충돌 
/*    
    for(int i=34;i<=41;i++)pinMode(i,INPUT);
      
    REG_PMC_PCER1= 1<<7;  
    REG_DMAC_WPMR=DMAC_WPMR_WPKEY(0x444d4143);
    REG_DMAC_EN=1;
    REG_DMAC_GCFG=0x00;
    REG_DMAC_EBCIER=1<<4;
    REG_DMAC_SADDR4=(uint32_t)TFT.pb;
    REG_DMAC_DADDR4=(uint32_t)&REG_SPI0_TDR;  
    REG_DMAC_DSCR4=0;
    REG_DMAC_CTRLB4=0x20310000;
    REG_DMAC_CFG4=  0x01412210;    
    
    REG_PIOA_PDR  = (1<<25)|(1<<27)|(1<<28);
    REG_PIOA_PER  = 1<<26;
    REG_PIOA_ABSR&=~((1<<25)|(1<<27)|(1<<28));
    REG_PMC_PCER0= 1<<24;
    REG_SPI0_WPMR=0x53504900; 
    REG_SPI0_CR=0x1;
    REG_SPI0_MR=0x00000011;
    SPI0->SPI_CSR[0]=0x00000080 + (xscale << 8);  
*/
}

void Tft::stopmono(){  /// TFT_F429_Discovery.h 와 충돌 
 /* 
  REG_DMAC_CHDR=1<<4;
  //while(REG_DMAC_CHSR&(1<<4));
  REG_DMAC_EBCIDR=1<<4;
  REG_SPI0_CR=0x0;
  REG_PIOA_PER  = 1<<26;
  */ 
}
void Tft::startcolour(){      /// TFT_F429_Discovery.h 와 충돌 
  /*  
  REG_PMC_PCER1= 1<<7;  
  REG_DMAC_WPMR=DMAC_WPMR_WPKEY(0x444d4143);
  REG_DMAC_EN=1;
  REG_DMAC_GCFG=0x00;
  REG_DMAC_SADDR5=(uint32_t)TFT.cb;
  REG_DMAC_DADDR5=(uint32_t)0x60000000;
  REG_DMAC_DSCR5=0;
  REG_DMAC_CTRLB5=0x20000000;
  REG_DMAC_CFG5=  0x10012200;
  
  REG_PMC_PCER0= 1<<9; 
  REG_PIOC_PDR=0b1111111100;
  REG_PIOC_ABSR&=~0b1111111100;  
  REG_SMC_WPCR=0x534d4300;
  REG_SMC_SETUP0=0x00000000;
  REG_SMC_PULSE0=0X00000101;
  REG_SMC_CYCLE0=xscale;
  REG_SMC_TIMINGS0=0;
  REG_SMC_MODE0=0x00000000;
  */      
}

void Tft::stopcolour() {  /// TFT_F429_Discovery.h 와 충돌 
  /*
  REG_PMC_PCDR0= 1<<9; 
  */
}

void Tft::dmapri() { /// TFT_F429_Discovery.h 와 충돌 
  /*
    // this code puts DMA priority above CPU.
  MATRIX->MATRIX_WPMR=0x4d415400;
  for(int i=0;i<6;i++)MATRIX->MATRIX_MCFG[i]=1;
  MATRIX->MATRIX_MCFG[4]=0;
  for(int i=0;i<8;i++)MATRIX->MATRIX_SCFG[i]=0x01000008;
  MATRIX->MATRIX_SCFG[6]=0x011200ff;
  MATRIX->MATRIX_PRAS0=0x00020100;
  MATRIX->MATRIX_PRAS1=0x00020100;
  MATRIX->MATRIX_PRAS2=0x00000000;
  MATRIX->MATRIX_PRAS3=0x00000003;
  MATRIX->MATRIX_PRAS4=0x00000000;
  MATRIX->MATRIX_PRAS5=0x00000000;
  MATRIX->MATRIX_PRAS6=0x00030000;
  MATRIX->MATRIX_PRAS7=0x00030000;
  MATRIX->MATRIX_PRAS8=0x00000100;
  */
}

int Tft::begin(int x, int y, int m)
{
  
  if(up)TFT.end();
  
  if(m!=TFT_MONO && m!=TFT_COLOUR) return -4;
  if(m==TFT_COLOUR && y>380)return -3;
  if(lfreqmin==0){lfreqmin=27000;lfreqmax=83000;ffreqmin=57;ffreqmax=70;}
  
  xsize=x;ysize=y;mode=m;
  tww=tw=xsize/8;twh=th=ysize/8;twx=twy=tx=ty=0;
  ink=255;paper=0;
  synced=0;framecount=0;line=linedouble=0;
    
  int r;
  r=calcmodeline(); if(r)return r;
  r=allocvideomem(); if(r)return r;
  
  dmapri();
  
  pinMode(_v_hsync,OUTPUT); 
  pinMode(_v_vsync,OUTPUT); 
  starttimers();

///  if(mode == TFT_TFT)startmono();
///  else if (mode == TFT_COLOUR)startcolour();
  startinterrupts();
  
  up=1;
  return 0;
}

int Tft::beginNTSC()
{
  mode=TFT_NTSC;
  xsize=320;ysize=200;tww=40;twh=25;twx=twy=tx=ty=0;
  ink=255;paper=0;
  synced=0;framecount=0;
  xscale=12;yscale=1;
  xtotal=444; xsyncstart=335; xsyncend=368;
  ytotal=262; ysyncstart=230; ysyncend=236;
  lfreq=15778; pclock=7000000; ltot=262;
  xclocks=5328; xstart=130;
  xsyncwidth=394;
  line=linedouble=0;
  phase=0;poff=8;
  
  int r;
  dmabuf=(uint16_t *)malloc(2048);
  crt[0]=(const uint16_t *)crtab;cbt[0]=cbtab;
  r=allocvideomem(); if(r)return r;  
  pinMode(_v_hsync,OUTPUT); 
  pinMode(_v_vsync,OUTPUT); 
  starttimers();
  dmapri();
/*  
  REG_PMC_PCER1= 1<<7;  
  REG_DMAC_WPMR=DMAC_WPMR_WPKEY(0x444d4143);
  REG_DMAC_EN=1;
  REG_DMAC_GCFG=0x00;
  REG_DMAC_SADDR5=(uint32_t)dmabuf;
  REG_DMAC_DADDR5=(uint32_t)0x60000000;
  REG_DMAC_DSCR5=0;
  REG_DMAC_CTRLB5=0x20000000;
  REG_DMAC_CFG5=  0x10702200;
  
  REG_PMC_PCER0= 1<<9; 
  REG_PIOC_PDR=0b1111111100;
  REG_PIOC_ABSR&=~0b1111111100;  
  REG_SMC_WPCR=0x534d4300;
  REG_SMC_SETUP0=0x00000000;
  REG_SMC_PULSE0=0X00000101;
  REG_SMC_CYCLE0=6;
  REG_SMC_TIMINGS0=0;
  REG_SMC_MODE0=0x00000000;
*/  
  startinterrupts();
  up=1;
  return 0;

}

void Tft::end()
{
  if(!up)return;
  up=0;
  
  stopinterrupts();
  if(mode == TFT_MONO)stopmono();
  else if (mode & TFT_COLOUR)stopcolour(); 
  
  stoptimers();
  pinMode(_v_hsync,INPUT); 
  pinMode(_v_vsync,INPUT); 
  
  freevideomem();
  if(mode==TFT_NTSC)free(dmabuf);
  
  pclock=xsize=xsyncstart=xsyncend=xtotal=ysize=ysyncstart=ysyncend=ytotal=0;
  mode=line=linedouble=synced=xclocks=xstart=xsyncwidth=xscale=yscale=0;
  lfreq=ffreq=ltot=0;

  return;
}



