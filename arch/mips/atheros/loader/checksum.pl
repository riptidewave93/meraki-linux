#!/usr/bin/perl

$checksum = 0;
$count = 0;

binmode(STDIN);
binmode(STDOUT);

while (read(STDIN, $buff, 4096)) {
    my @ints = unpack("N*", $buff);
    foreach (@ints) {
        $checksum ^= $_;
        $count += 4;
    }
}

print pack("NN", $count + 16, $checksum);

printf STDERR "checksummed %d bytes, final value 0x%x\n", $count, $checksum;

