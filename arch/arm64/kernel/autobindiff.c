/* autobindiff.c
 * author: Cel Skeggs <cela@meraki.net>
 */
#include <linux/types.h>
#include <linux/bug.h>
#include <asm/bitops.h>

/*
 * Given a bitmask, find the most significant bit that is both set and is one
 * bit more significant than a bit that is not set.
 *
 * Example:
 *     00000011111000000100000111110001
 *               ^      ^         ^   ^
 * The bits with a ^ would be possible results. Of these, the most significant
 * (leftmost) will actually be returned.
 *
 * 00000011111000000100000111110001 = x
 * 11111000001111110111110000011101 = ~(x << 1)
 * 00000000001000000100000000010001
 *
 * Returns an int in the range [0, 31].
 * Behavior is unspecified when the specified bitmask is zero.
 */
static unsigned int bitfields_max_lower_bound(unsigned int bitmask)
{
    unsigned int possible_bits = bitmask & ~(bitmask << 1);
    return fls(possible_bits) - 1;
}

static bool diff_word(unsigned int blob, unsigned int tmin, unsigned int tmax,
                      int *bitfield_out, unsigned int *bitfield_remaining)
{
    unsigned int optional_bits = tmin ^ tmax;
    unsigned int bitfields_left = optional_bits;
    unsigned int specified_left = blob & optional_bits;
    unsigned int min_left = tmin & optional_bits;
    unsigned int max_left = tmax & optional_bits;
    if ((blob & ~optional_bits) != (tmin & ~optional_bits))
        return false;
    while (bitfields_left) {
        unsigned int lower_bound = bitfields_max_lower_bound(bitfields_left);
        unsigned int upper_bound = fls(bitfields_left) - 1;
        bool sign_ext = (min_left >> lower_bound) > (max_left >> lower_bound);
        int value;
        if (!*bitfield_remaining)
            BUG();
        if (sign_ext)
            value = ((int) specified_left << (31 - upper_bound))
                    >> (31 - (upper_bound - lower_bound));
        else
            value = specified_left >> lower_bound;
        *bitfield_out++ = value;
        // limit to bits below the discovered point
        specified_left &= (1 << lower_bound) - 1;
        bitfields_left &= (1 << lower_bound) - 1;
        min_left &= (1 << lower_bound) - 1;
        max_left &= (1 << lower_bound) - 1;
        *bitfield_remaining -= 1;
    }
    return true;
}

/*
 * Compare a binary blob with a partial template, extracting the value of each
 * bitfield.
 *
 * The template is given in the form of two different templates: a minimum
 * template and a maximum template. Any bits that differ between these two
 * templates will be considered to be optional, and will pass validation
 * regardless of value. These templates are concatenated, and a pointer to the
 * concatenation is passed in the 'templates' parameter.
 *
 * Length is given as a count of 32-bit words.
 *
 * Each contiguous interval of bits that differ between the minimum template
 * and the maximum template (limited by 32-bit word boundaries) will be
 * returned through the bitfield_out parameter. The number of expected
 * bitfields should be passed in the bitfield_num parameter, so that a BUG()
 * can occur if the bitfield buffer is the wrong length.
 *
 * Return true on match, false on mismatch.
 */
bool autobindiff(void *blob, void *templates, unsigned long lenw,
                 unsigned int *bitfield_out, unsigned int bitfield_num)
{
    unsigned int *iblob = (unsigned int *) blob;
    unsigned int *tmin = (unsigned int *) templates;
    unsigned int *tmax = tmin + lenw;
    unsigned int remaining = bitfield_num;
    unsigned long i;
    for (i = 0; i < lenw; i++)
        if (!diff_word(iblob[i], tmin[i], tmax[i],
                       bitfield_out + bitfield_num - remaining, &remaining))
            return false;
    if (remaining)
        BUG();
    return true;
}
