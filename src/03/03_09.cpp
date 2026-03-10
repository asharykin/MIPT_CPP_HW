#include <assert.h>
#include <iostream>
#include <cstdint>

class IPv4
{
public:
    IPv4() : m_octets{0, 0, 0, 0} { }

    IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d)
    {
        m_octets[0] = a;
        m_octets[1] = b;
        m_octets[2] = c;
        m_octets[3] = d;
    }

    IPv4& operator++()
    {
        if (m_octets[0] < 255)
        {
            ++m_octets[0];
        } else
        {
            m_octets[0] = 0;
        }
        return *this;
    }

    IPv4& operator++(int)
    {
        if (m_octets[3] < 255)
        {
            ++m_octets[3];
        } else
        {
            m_octets[3] = 0;
        }
        return *this;
    }

    IPv4& operator--()
    {
        if (m_octets[0] > 0)
        {
            --m_octets[0];
        } else
        {
            m_octets[0] = 255;
        }
        return *this;
    }

    IPv4& operator--(int)
    {
        if (m_octets[3] > 0)
        {
            --m_octets[3];
        } else
        {
            m_octets[3] = 255;
        }
        return *this;
    }

    friend bool operator==(const IPv4& lhs, const IPv4& rhs)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (lhs.m_octets[i] != rhs.m_octets[i])
            {
                return false;
            }
        }
        return true;
    }

    friend bool operator!=(const IPv4& lhs, const IPv4& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator<(const IPv4& lhs, const IPv4& rhs)
    {
        for (int i = 0; i < 4; ++i)
            {
            if (lhs.m_octets[i] < rhs.m_octets[i])
            {
                return true;
            }
            if (lhs.m_octets[i] > rhs.m_octets[i])
            {
                return false;
            }
        }
        return false;
    }

    friend bool operator<=(const IPv4& lhs, const IPv4& rhs)
    {
        return lhs < rhs || lhs == rhs;
    }

    friend bool operator>(const IPv4& lhs, const IPv4& rhs)
    {
        return !(lhs <= rhs);
    }

    friend bool operator>=(const IPv4& lhs, const IPv4& rhs)
    {
        return !(lhs < rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const IPv4& ip)
    {
        os << static_cast<int>(ip.m_octets[0]) << "."
           << static_cast<int>(ip.m_octets[1]) << "."
           << static_cast<int>(ip.m_octets[2]) << "."
           << static_cast<int>(ip.m_octets[3]);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, IPv4& ip)
    {
        int a, b, c, d;
        char dot1, dot2, dot3;
        is >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d;
        if (dot1 == '.' && dot2 == '.' && dot3 == '.' &&
            a >= 0 && a <= 255 && b >= 0 && b <= 255 &&
            c >= 0 && c <= 255 && d >= 0 && d <= 255)
        {
            ip.m_octets[0] = static_cast<std::uint8_t>(a);
            ip.m_octets[1] = static_cast<std::uint8_t>(b);
            ip.m_octets[2] = static_cast<std::uint8_t>(c);
            ip.m_octets[3] = static_cast<std::uint8_t>(d);
        } else
        {
             std::cout << "Invalid IP address" << std::endl;
        }
        return is;
    }

private:
    std::uint8_t m_octets[4];
};

int main()
{
    IPv4 ip1;
    std::cin >> ip1;
    std::cout << "IP address from stdin: " << ip1 << std::endl;

    IPv4 ip2(127, 0, 0, 1);
    ++ip2;
    assert(ip2 == IPv4(128, 0, 0, 1));
    ip2++;
    assert(ip2 == IPv4(128, 0, 0, 2));

    --ip2;
    assert(ip2 == IPv4(127, 0, 0, 2));
    ip2--;
    assert(ip2 == IPv4(127, 0, 0, 1));

    IPv4 ip3 (130, 0, 0, 1);
    assert(ip2 < ip3 && ip2 <= ip3);
    assert(ip3 > ip2 && ip3 >= ip2);
    assert(ip2 != ip3);
}
