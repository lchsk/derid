#ifndef POS_HPP
#define POS_HPP

namespace derid {
class pos {
public:
  explicit pos(int row, int col);
  explicit pos();

  int get_row() const { return m_row; };
  int get_col() const { return m_col; };

  int set_row(int row) { m_row = row; }

  int set_col(int col) { m_col = col; }

private:
  int m_row, m_col;
};

} // namespace derid

#endif
