template <class address_t>
class circularBufferInfo
{
 public:
  address_t head;
  address_t tail;
  address_t capacity;
  bool isNotEmpty;
  
  circularBufferInfo()
  {
    clear();
  }

  circularBufferInfo(address_t cb_sz)
  {
    capacity = cb_sz;
    clear();
  }

  void clear()
  {
    head = 0;
    tail = 0;
    isNotEmpty = false;
  }

  address_t push()
  {
    if (head == tail) 
    {
      // circular buffer is full or empty
     if (isNotEmpty) if (++(head) >= capacity) head = 0;
    }
    address_t curr = tail;
    if (++tail >= capacity) tail = 0;
    isNotEmpty = true;
    return curr;
  }
  
  address_t prev(address_t curr)
  {
    address_t next = curr;
    if (next <= 0) {next = capacity;}
    return --next;
  }

  address_t next(address_t curr)
  {
    address_t next = ++curr;
    if (next >= capacity) {next = 0;}
    return next;
  }

  address_t item(address_t index)
  {
    return (head + index) % capacity;
  }

};


