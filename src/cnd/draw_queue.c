#include <cnd/draw_queue.h>
#include <cnd/xutils.h>

__asm__(
	".area .text\n\t"
	".bndry 256\n\t"
);
element_t       drawList[32];
queue_pointer_t drawQueue;

typedef union pos_t
{
    struct
    {
        i8 y;
        i8 x;
    };
    u16 value;
} pos_t, *pos;

void draw_stack_draw(void)
{
    queue_pointer_t it;
    it.element = drawList;

    while (it.idx != drawQueue.idx)
    {
        reset_0_ref(); // Reset to pos 0,0
        moveto_d(it.element->y, it.element->x);

        Vec_Misc_Count = U8(1 + *(i8*)it.element->data++);
        do
        {
            VIA_port_a = ((pos_t*)it.element->data)->y; // Send Y to A/D
            VIA_port_b = 0;   // Enable mux
            __asm__ volatile ("nop");
            ++VIA_port_b;
            VIA_port_a = ((pos_t*)it.element->data)->x;
            VIA_shift_reg = 0xFF; // Set solid line pattern
            VIA_t1_cnt_hi = 0x00; // Set scale
            
            it.element->data += sizeof(pos_t);

            while ((VIA_int_flags & 0x40) == 0) {} // Wait for Interrupt
            __asm__ volatile ("nop");
            VIA_shift_reg = 0x00;
        } while (--Vec_Misc_Count != 0);
        it.idx += (u8)sizeof(element_t);
    }
}

