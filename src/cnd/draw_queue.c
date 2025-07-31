#include <cnd/draw_queue.h>
#include <cnd/xutils.h>

__asm__(
	".area .text\n\t"
	".bndry 256\n\t"
);
element_t       drawList[24];
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

void draw_stack_clear(void)
{
	drawQueue.element = drawList;
}

void draw_stack_draw(void)
{
    pos_t* p, *end;
    queue_pointer_t it;
    it.element = drawList;

    while (it.idx != drawQueue.idx)
    {
        reset_0_ref();
        moveto_d(it.element->y, it.element->x);

        const i8 length = 1 + *(i8*)it.element->data++;
        p   = (pos)it.element->data;
        end = p + length;

        Vec_Misc_Count = (u8)length;

        do
        {
            pos_t q;
            q.value = p->value;
            VIA_port_a = q.y; // Send Y to A/D
            VIA_port_b = 0;   // Enable mux
            ++p;
            __asm__ volatile ("nop");
            ++VIA_port_b;
            VIA_port_a = q.x;
            VIA_shift_reg = 0xFF; // Set solid line pattern
            VIA_t1_cnt_hi = 0x00; // Set scale
            
            while ((VIA_int_flags & 0x40) == 0) {} // Wait for Interrupt

            __asm__ volatile ("nop");
            VIA_shift_reg = 0x00;

            //VIA_cntl      = 0xCC;
            //VIA_shift_reg = 0x00; // Redundant
        } while (--Vec_Misc_Count != 0);
        it.idx += (u8)sizeof(element_t);
    }
}