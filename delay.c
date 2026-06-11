#include"types.h"
//software delay in us
void delay_us(u32 tdly)

{

	tdly*=12;

	while(tdly--);

}
// software delay in ms
void delay_ms(u32 tdly)

{

	tdly*=12000;

	while(tdly--);
}
//software delay in s
void delay_s(u32 tdly)

{

	tdly*=12000000;

	while(tdly--);


}
